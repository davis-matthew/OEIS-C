/*
 * A133445  Write numbers in ternary under each other (right justified), read
 *          diagonals in SW-NE direction, sum digits.
 * https://oeis.org/A133445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 314, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{r=n-1}^{∞} digit_3(r, r-(n-1))
 * where digit_3(r, j) is the j‑th digit (0‑based from the right)
 * of r in base 3 (values 0, 1, 2).  The sum stops when j exceeds
 * the number of digits of r.
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t d = n - 1;              /* diagonal index (0‑based) */

    for (idx_t r = d; ; ++r) {
        idx_t j = r - d;          /* position of the digit in r */

        /* Count how many base‑3 digits r has. */
        idx_t len = 0;
        seq_t t = r;
        do {
            t = t / (seq_t)3;
            ++len;
        } while (t != 0);

        if (j >= len)            /* no more digits on this diagonal */
            break;

        /* Extract the j‑th digit (0‑based from the right). */
        t = r;
        for (idx_t k = 0; k < j; ++k)
            t = t / (seq_t)3;
        seq_t digit = t % (seq_t)3;

        sum += digit;
    }

    return sum;
}
