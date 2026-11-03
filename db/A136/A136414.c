/*
 * A136414  Put the natural numbers together without spaces and read them two
 *          at a time advancing one space each time.
 * https://oeis.org/A136414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = 10*A007376(n) + A007376(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9065 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t champerdigit(idx_t pos) {
    /* pos is 1‑based: pos >= 1 */
    idx_t d = 1;        /* number of digits of the current block */
    idx_t pow10 = 1;    /* 10^(d-1), the first number of the block */

    while (1) {
        /* block_len = number of digits in current block
           = 9 * pow10 * d; compute in 128‑bit to avoid overflow */
        unsigned __int128 block128 = (unsigned __int128)pow10 * 9 * d;
        if ((unsigned __int128)pos <= block128) {
            break; /* pos lies in this block */
        }
        pos -= (idx_t)block128;  /* remove whole block */
        d++;
        pow10 *= 10;             /* 10^(d-1) for next block */
    }

    /* Now pos lies within the d‑digit block starting with 'pow10' */
    idx_t index_in_block = (pos - 1) / d;          /* zero based index */
    idx_t offset = (pos - 1) % d;                 /* 0 = most significant digit */

    idx_t number = pow10 + index_in_block;        /* the actual number */

    /* Compute factor = 10^(d-1-offset) to isolate desired digit */
    idx_t factor = pow10;
    for (idx_t i = 0; i < offset; ++i) {
        factor /= 10;
    }

    seq_t digit = (seq_t)((number / factor) % 10);
    return digit;
}
/* where A007376(k) is the k‑th digit of the Champernowne constant
 * (the concatenation of natural numbers: 123456789101112...) */

seq_t a(idx_t n) {
    seq_t d1 = champerdigit(n);
    seq_t d2 = champerdigit(n + 1);
    return d1 * 10 + d2;
}
