/*
 * A194375  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(5) and < > denotes fractional part.
 * https://oeis.org/A194375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer m for which Sum_{k=1..m} (<1/2
 *            + k*sqrt(5)> - <k*sqrt(5)>) > 0. The sum can be expressed as 0.5
 *            * Σ_{k=1..m} s_k, where s_k = +1 if {k*sqrt(5)} < 0.5 and s_k =
 *            -1 otherwise. Thus m is in the sequence iff the running sum of
 *            s_k is positive. s_k can be decided usin...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* return square root floor of an unsigned __int128 */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64; /* exclusive upper bound */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return (uint64_t)(low - 1);
}

seq_t a(idx_t n)
{
    /* target index (1‑based) */
    idx_t target = n;
    idx_t k = 1;                /* current m */
    idx_t count = 0;            /* how many m found so far */
    seq_t last_m = 0;           /* the m that satisfies the condition for count */
    int64_t sum = 0;            /* running sum of s_k */

    while (count < target) {
        /* compute n_k = floor(k * sqrt(5)) using integer sqrt */
        unsigned __int128 prod = (unsigned __int128)5 * k * k;
        uint64_t n_k = isqrt_uint128(prod);

        /* evaluate (2*n_k + 1)^2 > 20*k^2 */
        unsigned __int128 l = (unsigned __int128)(2 * (unsigned __int128)n_k + 1);
        unsigned __int128 r = (unsigned __int128)20 * k * k;

        /* plus if inequality holds, else minus */
        if (l * l > r)
            sum += 1;
        else
            sum -= 1;

        if (sum > 0) {
            count++;
            last_m = k;
        }
        k++;
    }

    return (seq_t)last_m;
}
