/*
 * A023705  Numbers with no 0's in base-4 expansion.
 * https://oeis.org/A023705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers with no 0's in base‑4 expansion – the nth term * a(n) is
 *            the number obtained by taking the (n‑1)th integer in * base‑3,
 *            adding 1 to each base‑3 digit, and interpreting the result * as
 *            a base‑4 number. This enumerates all positive integers whose *
 *            base‑4 digits belong to {1,2,3}
 * Range:     u8 n <= 120, u16 n <= 9840, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to a zero‑based offset */
    idx_t rem = n - 1;

    /* Find the length L of the base‑4 number */
    seq_t pow3 = 3;   /* 3^1 */
    int L = 1;
    while (rem >= (idx_t)pow3) {
        rem -= (idx_t)pow3;   /* skip over all numbers of previous lengths */
        pow3 *= (seq_t)3;     /* next power of 3 */
        ++L;
    }

    /* Convert the offset (0 … 3^L−1) from base‑3 to base‑4 with digits+1 */
    seq_t result = 0;
    seq_t pow4   = 1;          /* 4^0 for the least significant digit */
    idx_t r = rem;
    for (int i = 0; i < L; ++i) {
        int digit = r % 3;     /* a digit in base‑3 (0,1,2) */
        r /= 3;
        result += (seq_t)(digit + 1) * pow4;
        pow4   *= (seq_t)4;
    }
    return result;
}
