/*
 * A004949  a(n) = round(n*phi^14), where phi is the golden ratio, A001622.
 * https://oeis.org/A004949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*phi^14) = 843*n - k, where phi^14 = (843 +
 *            377*sqrt(5))/2 and k = floor((843*n + 1 - 377*n*sqrt(5))/2) =
 *            round(n/phi^14), found with exact integer comparisons: k is the
 *            largest integer with (843*n + 1 - 2*k)^2 >= 710645*n^2
 * Range:     u8 n <= 0, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 rhs = (unsigned __int128)710645u * N * N;
    unsigned __int128 base = (unsigned __int128)843u * N + 1;
    unsigned long long k = (unsigned long long)(n / 843u) + 2;
    for (;;) {
        unsigned __int128 t = base - (unsigned __int128)2 * k;
        if (t * t >= rhs) break;
        k--;
    }
    return (seq_t)843 * (seq_t)n - (seq_t)k;
}
