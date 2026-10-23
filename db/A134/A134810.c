/*
 * A134810  Giza numbers.
 * https://oeis.org/A134810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   Giza numbers. For n = 1..9 a(n) = n. For n > 9, a(n) is an
 *            odd‑length palindrome whose digits, starting from the ends,
 *            increase by one toward the centre, such that the first digit
 *            ranges from 1 up to 10 - k, where k = (L+1)/2 is the number of
 *            digits up to the centre (i.e., L = 2*k - 1). There are...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 35, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* small n */
    if (n <= 9) return (seq_t)n;

    /* find the block k where the number lies */
    idx_t prev = 0;          /* cumulative count up to previous block */
    idx_t k = 1;             /* block index (k=1 => 1‑digit numbers) */
    for (;;) {
        idx_t block = 10 - k;          /* numbers in this block */
        if (block == 0) break;         /* no more numbers */

        idx_t next = prev + block;
        if (n <= next) {               /* found the correct block */
            idx_t pos = n - prev - 1;   /* 0‑based offset inside block */
            seq_t first_digit = (seq_t)(1 + pos);   /* the leftmost digit */
            idx_t L = 2 * k - 1;        /* total length of the palindrome */

            seq_t result = 0;
            for (idx_t i = 0; i < L; ++i) {
                seq_t digit;
                if (i < k) {
                    digit = first_digit + (seq_t)i;               /* left part */
                } else {
                    digit = first_digit + (seq_t)(L - 1 - i);      /* mirrored right part */
                }
                result = result * 10 + digit;
            }
            return result;
        }

        prev = next;
        ++k;
    }

    /* If n exceeds the finite sequence (n > 45), return 0 as a safe fallback. */
    return 0;
}
