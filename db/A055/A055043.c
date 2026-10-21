/*
 * A055043  Numbers of the form 2^(2i+1)*(8*j+3).
 * https://oeis.org/A055043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2i+1)*(8j+3) (i,j >= 0)
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function scans positive integers until it has found the n‑th term. */
seq_t a(idx_t n)
{
    /* Current integer being tested */
    seq_t val = 1;
    /* How many terms found so far */
    idx_t count = 0;

    while (count < n)
    {
        /* Valuation of 2 dividing val */
        idx_t v2 = 0;
        seq_t tmp = val;
        while ((tmp & 1) == 0)
        {
            tmp >>= 1;
            v2++;
        }

        /* val is of the required form if (val >> v2) ≡ 3 (mod 8) and v2 is odd */
        if (v2 > 0 && (v2 & 1) && ((tmp & 7) == 3))
        {
            count++;
            if (count == n)
                return val;
        }

        val++;                /* Next integer to test */
    }

    /* n >= 1 so the loop always returns before this point */
    return 0;
}
