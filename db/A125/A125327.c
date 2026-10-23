/*
 * A125327  Number of base 17 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(1) = 17, a(2) = 107, a(3) = 545, a(4) = 3235, a(5) =
 *            19487, a(6) = 120311, a(7) = 752895, a(8) = 4762851, a(9) =
 *            30376427, a(10) = 194992877, a(11) = 1258241573, a(12) =
 *            8153851075, a(13) = 53026892117, a(14) = 345868792705, a(15) =
 *            2261537247275. For n ≥ 16: a(n) = 17*a(n‑1) - 91*a(n...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial thirteen terms (a(0) … a(15)).  The values fit into any
       unsigned integer type used for seq_t.  If seq_t is a C++ big‑int
       class it should have a constructor from an integral constant. */
    static const seq_t init[16] = {
        (seq_t)1ULL,
        (seq_t)17ULL,
        (seq_t)107ULL,
        (seq_t)545ULL,
        (seq_t)3235ULL,
        (seq_t)19487ULL,
        (seq_t)120311ULL,
        (seq_t)752895ULL,
        (seq_t)4762851ULL,
        (seq_t)30376427ULL,
        (seq_t)194992877ULL,
        (seq_t)1258241573ULL,
        (seq_t)8153851075ULL,
        (seq_t)53026892117ULL,
        (seq_t)345868792705ULL,
        (seq_t)2261537247275ULL
    };

    /*  If n is within the first 16 terms we can return immediately. */
    if (n < 16)
        return init[n];

    /*  Use a fixed circular buffer of size 16 to hold the last 16
        computed values.  buffer[15] is a(n‑1), buffer[0] is a(n‑16). */
    seq_t buffer[16];
    for (idx_t i = 0; i < 16; ++i)
        buffer[i] = init[i];

    /*  Compute successive terms up to a(n).  The recurrence uses the
        last 15 values stored in buffer[15] … buffer[1].  After each
        computation the buffer is shifted one position left and the new
        value placed at buffer[15]. */
    for (idx_t k = 16; k <= n; ++k)
    {
        seq_t val = (seq_t)17 * buffer[15]
                  - (seq_t)91 * buffer[14]
                  + (seq_t)91 * buffer[13]
                  + (seq_t)540 * buffer[12]
                  - (seq_t)966 * buffer[11]
                  - (seq_t)1256 * buffer[10]
                  + (seq_t)2422 * buffer[9]
                  + (seq_t)1426 * buffer[8]
                  - (seq_t)2638 * buffer[7]
                  - (seq_t)845 * buffer[6]
                  + (seq_t)1379 * buffer[5]
                  + (seq_t)249 * buffer[4]
                  - (seq_t)327 * buffer[3]
                  - (seq_t)27 * buffer[2]
                  + (seq_t)27 * buffer[1];

        /* Shift the buffer left by one position. */
        for (idx_t i = 0; i < 15; ++i)
            buffer[i] = buffer[i + 1];
        buffer[15] = val;
    }

    return buffer[15];
}
