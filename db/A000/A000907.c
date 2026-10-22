/*
 * A000907  Second-order reciprocal Stirling number (Fekete) a(n) = [[2n+2,
 *          n]]. The number of n-orbit permutations of a (2n+2)-set with at
 *          least 2 elements in each orbit. Also known as associated Stirling
 *          numbers of the first kind (e.g., Comtet).
 * https://oeis.org/A000907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+5)*(2*n+2)!/(9*2^(n+1)*(n-1)!) =
 *            (4*n+5)*n*(n+1)*(2*n+1)!!/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    int need = 2; /* factors of 3 still to remove (the division by 9) */
    seq_t result = 1;
    uint64_t f;
    idx_t k;

    f = (uint64_t)(4 * n + 5);
    while (need > 0 && f % 3 == 0) { f /= 3; need--; }
    result *= (seq_t)f;

    f = (uint64_t)n;
    while (need > 0 && f % 3 == 0) { f /= 3; need--; }
    result *= (seq_t)f;

    f = (uint64_t)(n + 1);
    while (need > 0 && f % 3 == 0) { f /= 3; need--; }
    result *= (seq_t)f;

    for (k = 3; k <= 2 * n + 1; k += 2) {
        f = (uint64_t)k;
        while (need > 0 && f % 3 == 0) { f /= 3; need--; }
        result *= (seq_t)f;
    }

    return result;
}
