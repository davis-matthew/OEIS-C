/*
 * A064801  Take 1, skip 2, take 2, skip 3, take 3, etc.
 * https://oeis.org/A064801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Find the unique integer b with b(b+1)/2 ≥ n. The nth taken
 *            number equals b² + (n – b(b–1)/2 – 1)
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the “take 1, skip 2, take 2, skip 3, …” rule. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 for safety */

    /* Binary search for the block index b such that b(b+1)/2 ≥ n. */
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t b = low;
    /* Sum of numbers taken in previous blocks: (b-1)b/2. */
    unsigned __int128 prev_sum = (unsigned __int128)(b - 1) * b / 2;
    idx_t offset = n - (idx_t)prev_sum - 1;      /* 0‑based offset inside the current block */

    seq_t bb = (seq_t)b;
    seq_t result = bb * bb + (seq_t)offset;      /* b² + offset */
    return result;
}
