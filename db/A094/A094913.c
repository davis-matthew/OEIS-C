/*
 * A094913  Maximal number of distinct nonempty substrings of any binary
 *          string of length n.
 * https://oeis.org/A094913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Σ_{k=1..n} min(n-k+1, 2^k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n - k + 1;          /* number of positions for substrings of length k */
        seq_t add;
        if (k < 63) {                  /* 2^k fits in a 64‑bit shift. For larger k, 2^k > n. */
            seq_t p = ((seq_t)1) << k;
            add = (p <= (seq_t)m) ? p : (seq_t)m;
        } else {                       /* 2^k is definitely larger than m */
            add = (seq_t)m;
        }
        total += add;
    }
    return total;
}
