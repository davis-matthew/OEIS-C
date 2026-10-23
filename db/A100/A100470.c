/*
 * A100470  n appears A055642(n) times (appearances equal number of decimal
 *          digits).
 * https://oeis.org/A100470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the non‑decreasing left inverse of the digit position
 *            sequence; equivalently a(n) = the smallest integer m such that
 *            the total number of decimal digits of all integers 0 … m is > n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an incremental scan over digit blocks
   (0‑9, 10‑99, 100‑999, …).  For each block we keep the cumulative total
   of digits and stop when the target n lies inside the current block.
   The fraction part of n within the block directly gives the number. */

seq_t a(idx_t n) {
    /* Target position (in terms of seq_t to allow wide types). */
    seq_t target = n;
    seq_t total = 0;       /* digits counted so far */
    seq_t pow10 = 1;       /* will become 10^d after multiplication */
    int d = 0;             /* current digit length */

    while (1) {
        d++;                      /* move to next digit length block */

        /* Compute 10^d */
        pow10 *= 10;

        /* The first number of this block:
           0 for one‑digit block, 10^(d‑1) for longer blocks. */
        seq_t start = (d == 1) ? 0 : (pow10 / 10);

        /* Count of numbers in this block:
           10 for first block, 9*start for others. */
        seq_t count = (d == 1) ? 10 : (9 * start);

        /* Total digits contributed by this block. */
        seq_t block_digits = count * (seq_t)d;

        /* If target lies inside this block, compute the answer. */
        if (target < total + block_digits) {
            seq_t remaining = target - total;          /* offset inside block */
            seq_t index = remaining / (seq_t)d;       /* number of full blocks */
            return start + index;                     /* the required integer */
        }

        /* Otherwise move on to next block. */
        total += block_digits;
    }
}
