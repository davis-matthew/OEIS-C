/*
 * A045137  Numbers whose base-4 representation contains no 2's and exactly
 *          four 3's.
 * https://oeis.org/A045137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑4 representation contains no 2's and exactly
 *            four 3's. The function enumerates the numbers in ascending order
 *            by increasing base‑4 length
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for already generated terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Recursive helper that builds numbers of length L. */
static void generate_len_rec(size_t pos, size_t L, int chosen3, seq_t val)
{
    if (pos == L) {
        if (chosen3 == 4) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        return;
    }

    /* Remaining positions after this digit (including the current one) */
    size_t rem = L - (pos + 1);

    if (pos == 0) {
        /* Most significant digit: cannot be 0 */
        /* Try digit 1 */
        {
            int newChosen = chosen3;
            if (newChosen <= 4 && (4 - newChosen) <= (int)rem)
                generate_len_rec(pos + 1, L, newChosen, val * 4 + 1);
        }
        /* Try digit 3 */
        {
            int newChosen = chosen3 + 1;
            if (newChosen <= 4 && (4 - newChosen) <= (int)rem)
                generate_len_rec(pos + 1, L, newChosen, val * 4 + 3);
        }
    } else {
        /* Digits 0, 1, 3 in ascending order */
        /* Digit 0 */
        {
            int newChosen = chosen3;
            if (newChosen <= 4 && (4 - newChosen) <= (int)rem)
                generate_len_rec(pos + 1, L, newChosen, val * 4 + 0);
        }
        /* Digit 1 */
        {
            int newChosen = chosen3;
            if (newChosen <= 4 && (4 - newChosen) <= (int)rem)
                generate_len_rec(pos + 1, L, newChosen, val * 4 + 1);
        }
        /* Digit 3 */
        {
            int newChosen = chosen3 + 1;
            if (newChosen <= 4 && (4 - newChosen) <= (int)rem)
                generate_len_rec(pos + 1, L, newChosen, val * 4 + 3);
        }
    }
}

/* Generate all valid numbers of a given base‑4 length L. */
static void generate_len(size_t L)
{
    generate_len_rec(0, L, 0, 0);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is 1‑based */

    static size_t next_len = 4;       /* minimal length needed */
    while (cache_len < (size_t)n) {
        generate_len(next_len);
        ++next_len;
    }
    return cache[n - 1];
}
