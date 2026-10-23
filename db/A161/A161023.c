/*
 * A161023  Collatz trajectory starting at 35655.
 * https://oeis.org/A161023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 323, u64 n <= 323, u128 n <= 323,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Collatz trajectory starting at 35655.
   Formula: a(0)=35655; for n>0:
   if a(n‑1) even  then a(n)=a(n‑1)/2
   else            a(n)=3*a(n‑1)+1
   The sequence eventually enters the 4‑2‑1 cycle. */
#include <stddef.h> /* size_t */

seq_t a(idx_t n) {
    /* Cache up to a fixed maximum (much larger than the 10 000 limit). */
    static seq_t cache[20001];
    static size_t cache_len = 0;

    /* Initialise with the starting value on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)35655;
        cache_len = 1;
    }

    /* Extend cache up to the requested index. */
    while (cache_len <= (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t next;
        if ((prev & (seq_t)1) == 0)          /* even */
            next = prev / (seq_t)2;
        else                                 /* odd */
            next = prev * (seq_t)3 + (seq_t)1;

        cache[cache_len] = next;
        cache_len++;
    }

    return cache[(size_t)n];
}
