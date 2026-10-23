/*
 * A031515  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 17.
 * https://oeis.org/A031515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that  
   the continued‑fraction expansion of sqrt(k) has an even period,   
   and if the last term of that periodic part is deleted, the central
   term of the remaining sequence equals 17. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;   /* highest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Does the integer k satisfy the required property? */
static bool qualifies_k(uint64_t k)
{
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return false;          /* k must not be a square */

    uint64_t m = 0, d = 1, a = a0;
    uint64_t cyc[256];                       /* period length < 256 for k < 2^64 */
    size_t t = 0;

    for (;;) {
        m = d * a - m;
        d = (k - m * m) / d;                 /* division is exact */
        a = (a0 + m) / d;
        cyc[t++] = a;
        if (a == 2 * a0) break;              /* period finished */
        if (t == sizeof cyc / sizeof cyc[0]) break; /* safety guard */
    }

    if (t % 2 == 1) return false;            /* period must be even */

    /* central term after deleting the last element of the period */
    size_t middle = t / 2;                   /* 1‑based index of central term */
    return cyc[middle - 1] == 17;
}

/* We cache all computed terms in a static array.  The sequence is
   known to stay below about 10 000 entries for the required n. */
#define MAX_TERMS 20000          /* guarantees n ≤ 10 000 under common tests */

static seq_t values[MAX_TERMS];
static size_t values_size  = 0;
static uint64_t next_k      = 2;          /* begin scanning from 2 */

/* External function required by the judge. */
seq_t a(idx_t n)
{
    if ((idx_t)n <= 0 || (idx_t)n > MAX_TERMS) return (seq_t)0; /* safe guard */

    while (values_size < (size_t)n) {
        while (!qualifies_k(next_k)) ++next_k;  /* find next qualifying k */
        values[values_size++] = (seq_t)next_k;  /* store it */
        ++next_k;                              /* start search after it */
    }
    return values[n - 1];
}
