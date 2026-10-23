/*
 * A162908  Pairs of central divisors of A089229(n).
 * https://oeis.org/A162908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k-1) = A033676(A089229(k)), * a(2*k) = A033677(A089229(k)) *
 *            where A089229(k) is the k-th composite number that is not a
 *            perfect * square (the sequence starts 6, 8, 10, 12, 14, 15, 18,
 *            …). For each * such number m, we compute its two central
 *            divisors – the complementary * pair of divisors tha...
 * Range:     u8 n <= 76, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* ---------- cache for A089229 -------------------------------- */
static seq_t *A089229 = NULL;    /* array of cached values       */
static size_t A089229_len = 0;   /* number of elements stored    */
static size_t A089229_cap = 0;   /* allocated capacity           */
static idx_t   A089229_next = 6; /* first potential element      */

/* ---------- helpers ------------------------------------------- */
static inline bool is_square_seq(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Ensure that A089229 contains at least 'need' elements */
static void ensure_A089229(size_t need)
{
    while (A089229_len < need) {
        /* skip composites that are not perfect squares */
        for (;;) {
            idx_t cand = A089229_next;
            ++A089229_next;
            if (!is_prime(cand) && !is_square_seq((seq_t)cand)) {
                /* grow buffer if necessary */
                if (A089229_len == A089229_cap) {
                    size_t new_cap = A089229_cap ? A089229_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(A089229, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    A089229 = tmp;
                    A089229_cap = new_cap;
                }
                A089229[A089229_len++] = (seq_t)cand;
                break;
            }
        }
    }
}

/* ---------- main sequence function --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */

    /* position in A089229 */
    size_t k = (n + 1) / 2;               /* floor((n+1)/2)   */
    ensure_A089229(k);

    seq_t m = A089229[k - 1];             /* A089229(k)        */

    /* find central divisor d ≤ √m */
    seq_t sqrt_m = floor_sqrt(m);
    seq_t d = 1;
    for (idx_t i = (idx_t)sqrt_m; i >= 1; --i) {
        if (m % (seq_t)i == 0) {
            d = (seq_t)i;
            break;
        }
        if (i == 1) break;               /* avoid wraparound when idx_t is unsigned */
    }

    seq_t e = m / d;                      /* complementary divisor */

    return (n & 1) ? d : e;               /* odd → d, even → e */
}
