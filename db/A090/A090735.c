/*
 * A090735  Number of positive squarefree numbers <= n that can be expressed
 *          as a sum of 2 squares > 0.
 * https://oeis.org/A090735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the squarefree numbers ≤ n that are representable as
 *            a sum of two positive squares. A squarefree integer is such a
 *            sum iff it contains no prime divisor ≡ 3 (mod 4). This
 *            implementation builds a table of smallest prime factors up to n
 *            and then factors each number once
 * Range:     u8 n <= 1536, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>    /* malloc, free */

/* Smallest prime factor table */
static idx_t *spf = NULL;          /* array of length spf_limit + 1   */
static idx_t spf_limit = 0;        /* current maximal |spf| */

/* Build smallest‑prime‑factor table up to limit */
static void build_spf(idx_t limit)
{
    /* Allocate or re‑allocate the table */
    if (spf) free(spf);
    spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf) return;          /* out of memory – caller will treat as failure */

    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* Ensure spf table is built up to at least n */
static void ensure_spf(idx_t n)
{
    if (n > spf_limit) build_spf(n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(1) = 0 */

    ensure_spf(n);

    seq_t count = 0;
    for (idx_t num = 2; num <= n; ++num) {
        idx_t x = num;
        bool squarefree = true;
        bool bad_prime  = false;

        while (x > 1) {
            idx_t p = spf[x];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (exp > 1) {            /* not squarefree */
                squarefree = false;
                break;
            }
            if (p % 4 == 3) {         /* forbidden prime factor */
                bad_prime = true;
                break;
            }
        }
        if (squarefree && !bad_prime) ++count;
    }
    return count;
}
