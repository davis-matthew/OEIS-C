/*
 * A164577  Integer averages of the first perfect cubes up to some n^3.
 * https://oeis.org/A164577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(k+1)^2/4, where k is the n-th positive integer not
 *            congruent to 2 modulo 4 (i.e., k = 4*⌊(n-1)/3⌋ + {1,3,4}
 *            corresponding to (n-1) mod 3 = 0,1,2 respectively)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find k directly from n
    idx_t block = (n - 1) / 3;      // which block of 4 numbers
    idx_t inside = (n - 1) % 3;    // position inside the block (0,1,2)
    idx_t base = 4 * block;        // start of the block
    idx_t k;
    if (inside == 0) {
        k = base + 1;
    } else if (inside == 1) {
        k = base + 3;
    } else { /* inside == 2 */
        k = base + 4;
    }

    // Compute a(k) = k*(k+1)^2/4 without fractional intermediate
    seq_t kk = (seq_t)k;
    seq_t kp1 = kk + 1;  // k+1

    if ((k & 3) == 0) {              // k divisible by 4
        seq_t kdiv4 = kk / 4;
        return kdiv4 * kp1 * kp1;
    } else {                          // k mod 4 == 1 or 3
        seq_t t = kp1 / 2;            // (k+1)/2 is integer
        return kk * t * t;
    }
}
