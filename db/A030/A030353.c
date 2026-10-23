/*
 * A030353  Write 2n-1 in base 3 and juxtapose.
 * https://oeis.org/A030353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th digit of the concatenated base‑3 representations
 *            of the odd numbers 1, 3, 5, 7, … . That is, write 2k–1 in base 3
 *            for k=1,2,… and juxtapose the digits
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The input offset is 1‑based (OEIS offset 1).  We use seq_t for all
       arithmetic that may exceed the width of idx_t to avoid type mixing
       errors.  All intermediate results after type‑safe conversions are
       guaranteed to fit into idx_t (they are lengths or small indices
       only). */
    seq_t pos          = n;   /* 1‑based position in the digit sequence   */
    idx_t  d           = 1;   /* current number of digits in the block    */
    seq_t  pow3_prev   = 1;   /* 3^(d‑1) */
    seq_t  pow3        = 3;   /* 3^d   */

    while (1) {
        seq_t low  = (pow3_prev + 1) / 2;   /* ceil((3^(d-1)+1)/2)   */
        seq_t high = (pow3 - 1) / 2;        /* floor((3^d-1)/2)      */

        if (high < low) {          /* no numbers of this length – skip */
            d++;
            pow3_prev = pow3;
            pow3 *= 3;
            continue;
        }

        seq_t cnt          = high - low + 1;   /* number of d‑digit odds   */
        seq_t blockDigits  = cnt * d;         /* digits contributed       */

        if (pos <= blockDigits) {            /* target digit in this block */
            seq_t offset       = pos - 1;     /* 0‑based offset in block   */
            seq_t indexInBlock  = offset / d;/* which odd number in block */
            seq_t posInTermSeq = offset % d; /* 0‑based position in term  */

            /* The odd number whose digits we are inspecting */
            seq_t k   = low + indexInBlock;   /* the k-th odd number       */
            seq_t val = 2 * k - 1;            /* 2k-1 in decimal           */

            /* Find the digit at position posInTermSeq (leftmost = 0). */
            seq_t divisor = pow3;            /* start at 3^d              */
            idx_t  posInTerm = (idx_t)posInTermSeq;
            for (idx_t i = 0; i <= posInTerm; ++i)
                divisor /= 3;                /* divisor = 3^(d‑1‑posInTerm) */

            seq_t digit = (val / divisor) % 3;
            return digit;
        }

        /* The digit lies beyond this block – skip it */
        pos -= blockDigits;
        d++;
        pow3_prev = pow3;
        pow3 *= 3;
    }
}
