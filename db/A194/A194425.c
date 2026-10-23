/*
 * A194425  Numbers m such that Sum_{k=1..m} (<2/3 + k*r> - <k*r>) > 0, where
 *          r=sqrt(2) and < > denotes fractional part.
 * https://oeis.org/A194425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 2230, u32 n <= 2230, u64 n <= 2230,
 *            u128 n <= 2230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194425: a(n) = the n-th integer m such that
 *   Sum_{k=1..m} ( frac( 2/3 + k*sqrt(2) ) - frac( k*sqrt(2) ) ) > 0
 * where frac denotes the fractional part.
 *
 * The sum equals (3*A - m)/3 with A = #{k ≤ m : frac(k*sqrt(2)) ≤ 1/3}.
 * Thus the condition is A > m/3.
 *
 * We compute A incrementally.  Using the identity
 *   floor(k*sqrt(2)) = floor(sqrt(2*k*k)),
 * the condition frac(k*sqrt(2)) ≤ 1/3 is equivalent to
 *   floor(sqrt(18*k*k)) == 3*floor(sqrt(2*k*k)).
 *
 * Integer square roots are obtained by a binary‑search routine that
 * uses 128‑bit arithmetic safely.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root of a 64‑bit unsigned integer.
 * Returns the greatest r with r*r ≤ x.  Uses 128‑bit arithmetic to avoid overflow. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* hi > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) / 2;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Dynamic array to store the sequence values (m's satisfying the condition). */
static seq_t *seq_buf = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t  last_m   = 0;        /* last m processed */
static idx_t  A_count  = 0;        /* running count of acceptable k's */

/* Grow the dynamic array when needed. */
static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
    seq_t *new_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!new_buf) return;  /* on allocation failure leave unchanged – caller will not use the result */
    seq_buf = new_buf;
    seq_cap = new_cap;
}

/* Return the n-th term of the sequence (a(1) is the first). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        last_m++;   /* next m to test */
        uint64_t m = last_m;

        /* Compute floor( sqrt(2*m*m) ) and floor( sqrt(18*m*m) ). */
        uint64_t term1 = isqrt_u64(2 * m * m);
        uint64_t term2 = isqrt_u64(18 * m * m);

        /* Check whether frac(m*sqrt(2)) <= 1/3. */
        if (term2 == 3 * term1)
            ++A_count;

        /* Condition: A > m/3  <=> 3*A > m. */
        if (3 * A_count > m) {
            if (seq_len == seq_cap)
                grow_seq();
            seq_buf[seq_len++] = (seq_t)m;
        }
    }
    return seq_buf[n - 1];
}
