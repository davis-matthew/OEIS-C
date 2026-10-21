/*
 * A050256  a(n) = floor(47*(n-3/2)^(3/2)).
 * https://oeis.org/A050256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(47*(n-3/2)^(3/2))
 * Range:     u8 none, u16 n <= 3, u32 n <= 20, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = floor( sqrt( floor( 2209*(2n-3)^3 / 8 ) ) ) */

static seq_t int_sqrt(seq_t v)
{
    /* Generic integer square‑root (floor) by binary search */
    seq_t low = 0;
    seq_t high = v;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t mid2 = mid * mid;
        if (mid2 <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Behaviour for n < 2 is irrelevant; return 0 for safety */
    if (n < 2) return 0;

    /* u = 2*n - 3  */
    seq_t u = ((seq_t)2 * n) - 3;

    /* u^3 = u * u * u  */
    seq_t u2 = u * u;
    seq_t u3 = u2 * u;

    /* prod = 2209 * u^3 */
    seq_t prod = u3 * 2209ULL;   /* 2209 fits in uint64_t */

    /* div = floor(prod / 8)  */
    seq_t div = prod / 8;

    /* result = floor( sqrt(div) )  */
    return int_sqrt(div);
}
