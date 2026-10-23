/*
 * A101948  For any n >= 0 and b >= 2, let k be the length of the base-b
 *          expansion of n and let M(n, b) be the 2 X k matrix whose first row
 *          contains the first k primes in descending order and whose second
 *          row contains the base-b expansion of n. Let f(n, b) =
 *          determinant[transpose(M(n, b))*M(n, b)]. Sequence...
 * https://oeis.org/A101948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101948: a(n) = f(n,5) 
   f(n,b) is defined as  
     for 0 ≤ x < b:   f(x,b) = x*x + 4
     for n = y*b + x with 1 ≤ y < b, 0 ≤ x < b:
                    f(n,b) = 4*y*y + 9*x*x – 12*y*x
     for n ≥ b*b:     f(n,b) = 0
   Here b = 5. This function implements that definition directly. */
seq_t a(idx_t n)
{
    /* n < 5  →  n is a single digit in base 5 */
    if (n < 5) {
        seq_t t = (seq_t)n;
        return t * t + 4;
    }

    /* 5 ≤ n < 25  →  two base‑5 digits y·5 + x */
    if (n < 25) {
        seq_t y = (seq_t)(n / 5);          /* most significant digit (1…4) */
        seq_t x = (seq_t)(n % 5);          /* least significant digit (0…4) */
        return 4*y*y + 9*x*x - 12*y*x;
    }

    /* n ≥ 25  →  more than two digits – the determinant is zero */
    return 0;
}
