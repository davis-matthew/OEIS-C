/*
 * A199835  Number of -n..n arrays x(0..5) of 6 elements with zero sum and no
 *          two neighbors summing to zero.
 * https://oeis.org/A199835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n+1)*(2*n-1)*(66*n^2-35*n+19)/15
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for intermediate calculations to avoid overflow
     * This works on all GCC/Clang compilers that support the 128‑bit type.
     * The final result is cast back to seq_t, which may be 8, 16, 32, 64
     * or 128 bits or an arbitrary‑precision type. */
    unsigned __int128 nn = n;
    unsigned __int128 A = nn;
    unsigned __int128 B = 2*nn + 1;
    unsigned __int128 C = 2*nn - 1;
    unsigned __int128 D = 66*nn*nn - 35*nn + 19;

    /* Divide by 3 and 5 (the prime factors of 15) to keep the division exact */
    if (A % 3 == 0) {
        A /= 3;
    } else if (B % 3 == 0) {
        B /= 3;
    } else if (C % 3 == 0) {
        C /= 3;
    } else {
        D /= 3;
    }

    if (A % 5 == 0) {
        A /= 5;
    } else if (B % 5 == 0) {
        B /= 5;
    } else if (C % 5 == 0) {
        C /= 5;
    } else {
        D /= 5;
    }

    unsigned __int128 prod = 1;
    prod *= A;
    prod *= B;
    prod *= C;
    prod *= D;

    return (seq_t)prod;
}
