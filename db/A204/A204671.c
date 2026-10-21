/*
 * A204671  a(n) = n^n (mod 6).
 * https://oeis.org/A204671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n (mod 6)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Periodicity: a(0)=1, and for n>0, a(n) = a(n%6) with the cycle
   [1, 4, 3, 4, 5, 0] starting at n=1.) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */

    idx_t r = n % 6;       /* n > 0 so r ∈ {0,…,5} */

    /* Map the residue r to the corresponding value modulo 6 */
    switch ((int)r) {
        case 1: return 1;  /* a(1) = 1, a(7) = 1, ... */
        case 2: return 4;  /* a(2) = 4, a(8) = 4, ... */
        case 3: return 3;  /* a(3) = 3, a(9) = 3, ... */
        case 4: return 4;  /* a(4) = 4, a(10)= 4, ... */
        case 5: return 5;  /* a(5) = 5, a(11)= 5, ... */
        case 0: return 0;  /* a(6) = 0, a(12) = 0, ... */
    }
    return 0;              /* Unreachable, but keeps compiler happy */
}
