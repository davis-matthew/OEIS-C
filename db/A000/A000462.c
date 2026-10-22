/*
 * A000462  Numbers written in base of triangular numbers.
 * https://oeis.org/A000462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   greedy representation of n in the triangular-number base: with
 *            T(i) = i*(i+1)/2, take the largest k with T(k) <= n, then for i
 *            = k down to 1: d_i = floor(r / T(i)), r = r mod T(i); a(n) =
 *            Sum_{i=1..k} d_i * 10^(i-1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 65, u64 n <= 230, u128 n <= 819,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t remaining = n;

    /* Find the largest k such that T(k) = k*(k+1)/2 <= n */
    idx_t k = 1;
    for (;;) {
        idx_t next_tri = (k + 1) * (k + 2) / 2;
        if (next_tri > remaining)
            break;
        k++;
    }

    /* Extract digits from most significant to least significant */
    seq_t result = 0;
    idx_t i;
    for (i = k; i >= 1; i--) {
        idx_t tri = i * (i + 1) / 2;
        idx_t digit = remaining / tri;
        remaining = remaining % tri;
        result = result * 10 + (seq_t)digit;
    }

    return result;
}
