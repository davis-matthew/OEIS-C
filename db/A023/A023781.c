/*
 * A023781  Metadromes: digits in base 13 are in strict ascending order.
 * https://oeis.org/A023781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 1207, u32 n <= 4017, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Metadromes: numbers whose base‑13 digits are strictly increasing */

#include <stdint.h>

/* binomial coefficient C(n,k) for 0 <= k <= n <= 12.
   returns exact result in seq_t, using only integer arithmetic. */
static seq_t choose(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;   /* exact division */
    return res;
}

/* a(n): the n-th smallest non‑negative integer whose base‑13 digits
   are in strict ascending order (sorted by decimal value). */
seq_t a(idx_t n)
{
    /* total number of such numbers:
       13 single‑digit numbers (0..12) +
       C(12,2)+C(12,3)+...+C(12,12) numbers with 2+ digits. */
    static const idx_t MAX_TOTAL = 4096ULL;   /* 13 + sum_{k=2}^{12} C(12,k) */

    if (n < 1) return 0;                     /* out of defined range */

    if (n <= 13)                        /* 0 .. 12 */
        return (seq_t)(n - 1);

    if (n > MAX_TOTAL)                  /* sequence ends at 4096 */
        return 0;

    /* rank among numbers with length >= 2 (1‑based) */
    idx_t rem = n - 13;
    idx_t len = 2;

    /* find the length of the number for current rank */
    for (;; ++len)
    {
        idx_t cnt = (idx_t)choose(12, len); /* count of length‑len numbers */
        if (rem <= cnt)
            break;
        rem -= cnt;
    }

    idx_t rank = rem - 1;                /* zero‑based rank within this length */

    int digits[13];
    idx_t cur_rank = rank;
    int start = 1;                       /* first digit cannot be 0 for len>1 */

    /* unrank the combination of digits */
    for (idx_t pos = 0; pos < len; ++pos)
    {
        for (int d = start; d <= 12 - (len - pos - 1); ++d)
        {
            idx_t c = (idx_t)choose(12 - d, len - pos - 1);
            if (cur_rank < c)
            {
                digits[pos] = d;
                start = d + 1;
                break;
            }
            cur_rank -= c;
        }
    }

    /* evaluate base‑13 number */
    seq_t value = 0;
    for (idx_t i = 0; i < len; ++i)
        value = value * 13 + digits[i];

    return value;
}
