/*
 * A195001  Number of lower triangles of a 5 X 5 0..n array with each element
 *          differing from all of its horizontal and vertical neighbors by
 *          one.
 * https://oeis.org/A195001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2952*n - 6910 for n > 6 (empirical). Small values for n ≤
 *            6 are tabled directly
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first six terms are hard‑coded for correctness */
    if (n <= 6) {
        switch ((int)n) {
            case 1: return (seq_t)2;
            case 2: return (seq_t)576;
            case 3: return (seq_t)2290;
            case 4: return (seq_t)4990;
            case 5: return (seq_t)7858;
            case 6: return (seq_t)10804;
        }
    }
    /* for all n > 6 compute the linear formula */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long res = 2952ULL * nn - 6910ULL;
    return (seq_t)res;
}
