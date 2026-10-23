/*
 * A166038  Period of A022405 mod n.
 * https://oeis.org/A166038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of A022405 modulo n.  The period is the length of the cycle of
   the 3‑term state (b(n‑3),b(n‑2),b(n‑1)) under the recurrence
   b(k) = b(k‑1)*b(k‑2) – b(k‑3) modulo n, with initial values
   b(0)=0, b(1)=1, b(2)=2. */
seq_t a(idx_t n)
{
    idx_t m = n;                /* modulus (n ≥ 1) */
    idx_t x = 0 % m;            /* b(n‑3)  */
    idx_t y = 1 % m;            /* b(n‑2)  */
    idx_t z = 2 % m;            /* b(n‑1)  */

    /* remember the initial triple */
    idx_t orig_x = x, orig_y = y, orig_z = z;

    seq_t period = 0;

    do
    {
        /* compute next state */
        idx_t nx = y;
        idx_t ny = z;

        /* (y * z - x) modulo m; compute with 64‑bit arithmetic to avoid overflow */
        unsigned long long prod = ((unsigned long long)y * z) % m;
        unsigned long long nz = (prod + m - x) % m;

        /* update state */
        x = nx;
        y = ny;
        z = (idx_t)nz;

        period++;
    }
    while (!(x == orig_x && y == orig_y && z == orig_z));

    return period;
}
