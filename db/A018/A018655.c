/*
 * A018655  Divisors of 804.
 * https://oeis.org/A018655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 804 in increasing order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define NUM_DIVERS 12

seq_t a(idx_t n)
{
    static seq_t divs[NUM_DIVERS];
    static int init = 0;

    if (!init)
    {
        int pos = 0;
        for (int e2 = 0; e2 <= 2; ++e2)
        {
            seq_t pow2 = 1;
            for (int i = 0; i < e2; ++i) pow2 *= 2;

            for (int e3 = 0; e3 <= 1; ++e3)
            {
                seq_t pow3 = pow2;
                for (int i = 0; i < e3; ++i) pow3 *= 3;

                for (int e67 = 0; e67 <= 1; ++e67)
                {
                    seq_t d = pow3;
                    if (e67) d *= 67;
                    divs[pos++] = d;
                }
            }
        }

        /* simple insertion sort (exactly 12 elements) */
        for (int i = 1; i < NUM_DIVERS; ++i)
        {
            seq_t key = divs[i];
            int j = i - 1;
            while (j >= 0 && divs[j] > key)
            {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }

        init = 1;
    }

    if (n == 0 || n > NUM_DIVERS)
        return 0;
    return divs[n - 1];
}
