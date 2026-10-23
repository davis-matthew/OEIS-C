/*
 * A030548  Write n in base 6 and juxtapose.
 * https://oeis.org/A030548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   a(n) is the n‑th digit of the Champernowne constant formed by
 *            concatenating the base‑6 representations of the positive
 *            integers 1, 2, 3, … (i.e. a(n) = the nth digit in that infinite
 *            word). The algorithm processes the blocks of numbers having the
 *            same number of base‑6 digits
 * Range:     u8 n <= 95, u16 n <= 5950, u32 n <= 5950, u64 n <= 5950,
 *            u128 n <= 5950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* n is 1‑based; if n is 0 return 0 (undefined but harmless) */
    if (n == 0) return 0;

    /* Work with seq_t for large values and idx_t for loop counters. */
    seq_t idx = (seq_t)n;          /* copy of the index for comparisons */
    idx_t length = 1;              /* number of base‑6 digits in this block */
    seq_t power = (seq_t)6;        /* 6^length  */
    seq_t cumulative = 0;          /* digits processed so far (excluding current block) */

    while (1) {
        seq_t count = (seq_t)5 * (power / (seq_t)6);          /* numbers in this block: 5·6^(length‑1) */
        seq_t block_digits = count * (seq_t)length;          /* digits contributed by this block */

        /* Is the desired position in this block? */
        if (idx <= cumulative + block_digits) {
            /* Position inside the block (1‑based) */
            seq_t offset_in_block = idx - cumulative;

            /* Which number in this block and which digit within it? */
            seq_t number_index_seq = (offset_in_block - (seq_t)1) / (seq_t)length;
            seq_t digit_index_seq   = (offset_in_block - (seq_t)1) % (seq_t)length;

            /* The first number in this block is 6^(length‑1) */
            seq_t start_number = power / (seq_t)6;
            seq_t the_number   = start_number + number_index_seq;

            /* Compute 6^(length‑1‑digit_index) to isolate the desired digit */
            idx_t exp = (idx_t)(length - 1) - (idx_t)digit_index_seq;
            seq_t divisor = 1;
            for (idx_t i = 0; i < exp; ++i)
                divisor *= (seq_t)6;

            seq_t digit = (the_number / divisor) % (seq_t)6;
            return digit;
        }

        /* Move to the next block */
        cumulative += block_digits;
        ++length;
        power *= (seq_t)6;
    }
}
