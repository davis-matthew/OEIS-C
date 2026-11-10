/*
 * A205863  The least number j such that 7 divides s(k)-s(j), where
 *          k(n)=A205862(n) and s(k) denotes the (k+1)-st Fibonacci number.
 * https://oeis.org/A205863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least j with s(k) ≡ s(j) (mod 7), where k = A205862(n)
 *            and s(m) = Fibonacci(m+1); computed by direct enumeration of the
 *            pairs (k,j), j<k, in order of k then j
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char A205863_res[16] =
    {1,2,3,5,1,6,0,6,6,5,4,2,6,1,0,1};

seq_t a(idx_t n)
{
    idx_t counts[7] = {0, 0, 0, 0, 0, 0, 0};
    idx_t cumulative = 0;
    idx_t k = 1;

    for (;;)
    {
        unsigned int r = A205863_res[(k - 1) & 15];
        idx_t m = counts[r];
        if (cumulative + m >= n)
        {
            /* we need the (n - cumulative)-th index j with residue r (ascending) */
            idx_t want = n - cumulative;
            idx_t seen = 0;
            for (idx_t j = 1; j < k; ++j)
            {
                if (A205863_res[(j - 1) & 15] == r)
                {
                    ++seen;
                    if (seen == want)
                        return (seq_t)j;
                }
            }
            return (seq_t)0; /* unreachable */
        }
        cumulative += m;
        counts[r] = m + 1;
        ++k;
    }
}
