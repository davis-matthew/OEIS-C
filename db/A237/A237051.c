/*
 * A237051  Smallest set of distinct numbers such that the sum of their
 *          reciprocals is 1 and each number is of the form p^i*q^j where p
 *          and q are distinct primes and i and j are positive.
 * https://oeis.org/A237051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237051: the 20 distinct numbers of the form p^i*q^j
 * (with i,j>0 and distinct primes p,q) whose reciprocals sum to 1.
 * These are the unique minimal such set:
 * 6, 10, 12, 14, 15, 18, 20, 21, 22, 24, 26, 28, 33, 35, 36, 39, 40, 44, 52, 55.
 * The sequence is defined only for 1 ≤ n ≤ 20; out‑of‑range indices return 0. */

seq_t a(idx_t n)
{
    static const seq_t set20[20] = {6,10,12,14,15,18,20,21,22,24,26,28,33,35,36,39,40,44,52,55};
    if (n == 0) return 0;          /* indices start at 1, but guard against 0. */
    if (n <= 20) return set20[n - 1];
    return 0;                      /* defined only for n ≤ 20. */
}
