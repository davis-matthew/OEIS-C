/*
 * A202271  Right-truncatable perfect powers: every prefix is perfect power.
 * https://oeis.org/A202271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the nth right‑truncatable perfect power (OEIS A202271)
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   floor_sqrt, mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"
#include "../utils/floor_sqrt.h"
#include "../utils/mul_small.h"

/* --- cache ----------------------------------------------------------- */
static seq_t *seq = NULL;      /* array of found terms */
static size_t seq_len = 0;     /* number of terms collected */
static size_t seq_cap = 0;     /* current allocated capacity */

/* ensure we have capacity for at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* test whether x (>0) is a perfect power.
   Uses repeated integer exponentiation and a binary search for the base. */
static bool is_perfect_power(seq_t x)
{
    if (x == (seq_t)1) return true;      /* 1 = 1^k */

    seq_t pow2 = (seq_t)4;          /* 2^2 */
    int e = 2;
    while (pow2 <= x)
    {
        /* upper bound for the base is floor_sqrt(x) */
        seq_t limit = floor_sqrt(x);
        for (seq_t base = (seq_t)2; base <= limit; ++base)
        {
            seq_t p = pow_seq(base, e);
            if (p == (seq_t)0 || p > x) break;   /* overflow or too large */
            if (p == x) return true;
        }
        /* advance exponent: pow2 *= 2  (2^e becomes 2^(e+1)) */
        pow2 = mul_small(pow2, (unsigned)2);
        ++e;
    }
    return false;
}

/* build the entire finite sequence once */
static void build_sequence(void)
{
    if (seq) return;                /* already built */

    ensure_capacity(16);
    seq_len = 0;

    /* one‑digit perfect powers: 1,4,8,9 */
    seq[seq_len++] = (seq_t)1;
    seq[seq_len++] = (seq_t)4;
    seq[seq_len++] = (seq_t)8;
    seq[seq_len++] = (seq_t)9;

    /* breadth‑first search for extensions */
    for (size_t i = 0; i < seq_len; ++i)
    {
        seq_t cur = seq[i];
        for (int d = 0; d <= 9; ++d)
        {
            seq_t cand = cur * (seq_t)10 + (seq_t)d;
            if (is_perfect_power(cand))
            {
                ensure_capacity(seq_len + 1);
                seq[seq_len++] = cand;
            }
        }
    }
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1 */
    build_sequence();
    if ((idx_t)n > seq_len) return (seq_t)0;
    return seq[n - 1];
}
