/*
 * A023809  Xenodromes: all digits in base 15 are different.
 * https://oeis.org/A023809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th non‑negative integer whose base‑15 representation
 *  contains only distinct digits.  The function enumerates the numbers
 *  using a combinatorial rank/de‑rank approach.
 */
#include <stdbool.h>

/* Number of length‑k distinct‑digit base‑15 numbers.
 * For k=1  : 15 (0…14)
 * For k>1 : 14 * P(14, k‑1) , first digit can be 1‑14, the rest
 *          are permutations of the remaining 14 digits.  */
static uint64_t distinct_block_size(int k)
{
    if (k == 1) return 15ULL;          /* 0…14 */
    uint64_t prod = 14ULL;             /* first digit (1‑14) */
    for (int i = 1; i < k; ++i)       /* remaining k‑1 digits */
        prod *= (14ULL - (i - 1));
    return prod;                       /* 14 * P(14, k‑1) */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined, but safe */

    /* zero‑based rank of the desired number */
    uint64_t rem = n - 1;
    int k = 1;

    /* Find the digit length k of the n‑th number. */
    while (1) {
        uint64_t block = distinct_block_size(k);
        if (rem < block)
            break;
        rem -= block;
        ++k;
    }

    bool used[15] = { false };
    int used_cnt = 0;
    seq_t value = 0;

    for (int pos = 1; pos <= k; ++pos) {
        /* first digit: cannot be 0 when k>1; otherwise allow 0. */
        int start_d = (pos == 1) ? ((k == 1) ? 0 : 1) : 0;

        for (int d = start_d; d < 15; ++d) {
            if (used[d])
                continue;

            int remaining_positions = k - pos;
            uint64_t avail = 15 - used_cnt;  /* still unused digits */

            /* Count how many numbers can be formed after choosing d. */
            uint64_t ways = 1;
            for (int i = 0; i < remaining_positions; ++i)
                ways *= (avail - 1 - i);

            if (rem >= ways) {
                rem -= ways;
                continue;                   /* skip digit d */
            }

            /* Choose digit d. */
            value = value * 15 + (seq_t)d;
            used[d] = true;
            ++used_cnt;
            break;
        }
    }
    return value;
}
