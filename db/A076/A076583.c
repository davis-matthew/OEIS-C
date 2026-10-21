/*
 * A076583  Triangular numbers which are 9-almost primes.
 * https://oeis.org/A076583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth triangular number T = m(m+1)/2 whose prime
 *            factorisation contains exactly nine prime factors counted with
 *            multiplicity (i.e. Ω(T)=9). The routine iterates over m=1,2,3,…,
 *            testing this condition by factoring m and m+1 (or m/2 and m+1
 *            when m is even). A smallest‑prime‑factor array u...
 * Range:     u8 none, u16 n <= 2, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SPF 15000000U   /* maximum value that can appear in a factor */

/* smallest prime factor table, built on first use */
static unsigned int *spf = NULL;

/* ---- build SPF table ----------------------------------------------------- */
static void init_spf(void)
{
    if (spf != NULL) return;    /* already built */

    const size_t sz = (size_t)MAX_SPF + 1;
    spf = (unsigned int *)malloc(sz * sizeof(unsigned int));
    if (!spf) return;           /* allocation failed – assumed impossible */

    for (size_t i = 0; i < sz; ++i) spf[i] = (unsigned int)i;

    for (size_t i = 2; i * i <= MAX_SPF; ++i) {
        if (spf[i] == (unsigned int)i) {            /* i is prime */
            for (size_t j = i * i; j <= MAX_SPF; j += i)
                if (spf[j] == (unsigned int)j) spf[j] = (unsigned int)i;
        }
    }
}

/* ---- count Ω(x) (prime factors with multiplicity) ---------------------- */
static int count_bigomega(unsigned int x)
{
    int cnt = 0;
    while (x > 1) {
        ++cnt;
        x /= spf[x];
    }
    return cnt;
}

/* ---- public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_spf();
        initialized = true;
    }

    int found = 0;     /* how many qualifying triangulars have been found */

    for (idx_t m = 1; ; ++m) {
        /* triangular number T_m = m(m+1)/2 */
        seq_t triangle = (seq_t)m * ((seq_t)m + 1) / 2;

        /* factorise the two coprime parts of m(m+1)/2 */
        unsigned int part1, part2;
        if ((m & 1) == 0) {          /* m even */
            part1 = (unsigned int)(m / 2);
            part2 = (unsigned int)(m + 1);
        } else {                     /* m odd */
            part1 = (unsigned int)m;
            part2 = (unsigned int)((m + 1) / 2);
        }

        /* test Ω(part1)+Ω(part2) == 9 */
        int omega = count_bigomega(part1) + count_bigomega(part2);
        if (omega == 9) {
            ++found;
            if ((idx_t)found == n) return triangle;
        }
    }
}
