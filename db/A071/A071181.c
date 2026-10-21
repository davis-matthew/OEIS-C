/*
 * A071181  Number of numbers k such that phi(k) divides phi(n).
 * https://oeis.org/A071181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 778, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071181 – Number of integers k with φ(k) | φ(n)  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- small primes for factorising φ(n) (φ(n) ≤ 4000 for n ≤ 10000) ---------- */
static const int small_primes[25] = {
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97
};

/* ---------- pre‑computation limits ------------------------------------------- */
#define PRECOMP_LIMIT 200000   /* φ(k) for all k ≤ PRECOMP_LIMIT is pre‑stored */

/* ---------- global tables ---------------------------------------------------- */
static uint32_t *phi_arr = NULL;          /* φ(k) for k=0…PRECOMP_LIMIT            */
static seq_t   *cnt_tot = NULL;          /* cnt_tot[v] = #{k ≤ PRECOMP_LIMIT : φ(k)=v} */
static bool precomp_done = false;         /* one‑time initialisation flag          */

/* ---------- recursive divisor generation ------------------------------------ */
static void gen_divisors(int idx, unsigned long long cur,
                         int fcnt, const int *pr, const int *exp,
                         unsigned long long *out, int *ocnt)
{
    if (idx == fcnt) {
        out[(*ocnt)++] = cur;
        return;
    }
    unsigned long long val = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors(idx + 1, cur * val, fcnt, pr, exp, out, ocnt);
        val *= (unsigned long long)pr[idx];
    }
}

/* ---------- pre‑compute φ(k) and the count of k for each totient ---------------- */
static void precompute(void)
{
    if (precomp_done) return;
    precomp_done = true;

    phi_arr = (uint32_t *)malloc(sizeof(uint32_t) * (PRECOMP_LIMIT + 1));
    cnt_tot = (seq_t *)malloc(sizeof(seq_t) * (PRECOMP_LIMIT + 1));
    if (!phi_arr || !cnt_tot) return;   /* allocation failure – should not happen */

    /* initialise φ values to index */
    for (size_t i = 0; i <= (size_t)PRECOMP_LIMIT; ++i) {
        phi_arr[i] = (uint32_t)i;
        cnt_tot[i] = 0;
    }

    /* Euler’s totient sieve */
    for (int i = 2; i <= PRECOMP_LIMIT; ++i) {
        if (phi_arr[i] == (uint32_t)i) {          /* i is prime */
            for (int j = i; j <= PRECOMP_LIMIT; j += i) {
                phi_arr[j] -= phi_arr[j] / (uint32_t)i;
            }
        }
    }

    /* count occurrences of each totient value up to the limit */
    for (int k = 1; k <= PRECOMP_LIMIT; ++k) {
        uint32_t d = phi_arr[k];
        if (d <= (uint32_t)PRECOMP_LIMIT) {
            cnt_tot[d] += (seq_t)1;
        }
    }
}

/* ---------- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    precompute();                 /* ensure tables are ready */

    if (n > (idx_t)PRECOMP_LIMIT)     /* out of pre‑computation range – should not occur */
        return 0;

    unsigned long long phi_n = (unsigned long long)phi_arr[(size_t)n];

    /* factorise φ(n) – φ(n) ≤ 4000 for n ≤ 10000, so small_primes suffice */
    int fac_pr[25], fac_exp[25], fcnt = 0;
    unsigned long long tmp = phi_n;
    for (int i = 0; i < 25 && (unsigned long long)small_primes[i] * (unsigned long long)small_primes[i] <= tmp; ++i) {
        int p = small_primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac_pr[fcnt] = p;
            fac_exp[fcnt] = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        fac_pr[fcnt] = (int)tmp;
        fac_exp[fcnt] = 1;
        ++fcnt;
    }

    /* generate all divisors of φ(n) */
    unsigned long long divisors[256];
    int divcnt = 0;
    gen_divisors(0, 1ULL, fcnt, fac_pr, fac_exp, divisors, &divcnt);

    /* sum counts of k with φ(k) equal to any divisor of φ(n) */
    seq_t result = 0;
    for (int i = 0; i < divcnt; ++i) {
        unsigned long long d = divisors[i];
        if (d <= (unsigned long long)PRECOMP_LIMIT) {
            result += cnt_tot[(size_t)d];
        }
    }

    return result;
}
