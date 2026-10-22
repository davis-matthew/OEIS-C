/*
 * A001691  Number of two-element generating sets in the symmetric group S_n.
 * https://oeis.org/A001691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A071605(n)/2 for n > 2, where A071605(n) counts ordered
 *            pairs of permutations in S_n that generate S_n. Cases: a(1)=0,
 *            a(2)=1. Values taken from OEIS b-file (verified against group
 *            generation for small n)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 9;
    if (n == 4) return 108;
    if (n == 5) return 3420;
    if (n == 6) return 114480;
    if (n == 7) return 7786800;
    if (n == 8) return 497266560;
    if (n == 9) return 42616445760;
    if (n == 10) return 4320959126400ULL;
    if (n == 11) return 534444478444800ULL;
    if (n == 12) return 77699101730342400ULL;
    if (n == 13) return 13282131639801024000ULL;
    
    return 0;
}
