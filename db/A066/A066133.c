/*
 * A066133  Numbers that in base 2 need twelve 'Reverse and Add' steps to
 *          reach a palindrome.
 * https://oeis.org/A066133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066133: numbers that in base 2 need exactly 12
   reverse‑and‑add steps to reach a binary palindrome
   (palindromes themselves are excluded).
   The following code searches for such numbers by brute force,
   caching results for repeated calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Reverse the binary representation of x.
   Leading zero bits in the reversed number are discarded
   in the usual numeric conversion. */
static seq_t reverse_bits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = (rev << 1) | (x & (seq_t)1);
        x >>= 1;
    }
    return rev;
}

/* Dynamic cache for found terms. */
static seq_t *term_cache = NULL;
static idx_t term_count = 0;      /* number of terms cached   */
static idx_t cache_cap   = 0;      /* allocated array capacity */
static idx_t next_candidate = 1;   /* next integer to test     */

/* Grow the cache to accommodate at least `needed` entries. */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap < 8 ? 8 : cache_cap * 2;
    while (new_cap < (size_t)needed) new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap = (idx_t)new_cap;
}

/* Find and store the next term that needs exactly 12 reverse‑and‑add
   steps to become a palindrome.  The function updates
   `next_candidate`, `term_cache`, and `term_count`. */
static void compute_next_term(void)
{
    for (;; ++next_candidate) {
        seq_t x = (seq_t)next_candidate;
        seq_t rev = reverse_bits(x);
        idx_t steps = 0;
        bool bad = false;

        /* Repeat until a palindrome is reached or steps > 12. */
        while (x != rev) {
            if (steps >= 12) {         /* would exceed 12 steps */
                bad = true;
                break;
            }
            /* Add x and its binary reverse. */
            if (sizeof(seq_t) <= 8) {   /* 64‑bit or less: guard against overflow. */
                unsigned __int128 sum128 = (unsigned __int128)x + (unsigned __int128)rev;
                if ((uint64_t)sum128 < x) {    /* wrapped around */
                    bad = true;
                    break;
                }
                x = (seq_t)sum128;
            } else {                    /* arbitrary precision: addition works natively. */
                x += rev;
            }
            steps++;
            rev = reverse_bits(x);
        }

        if (!bad && steps == 12 && x == rev) {
            /* Store the found term. */
            grow_cache(term_count + 1);
            term_cache[term_count++] = (seq_t)next_candidate;
            ++next_candidate;          /* start searching after this number next time */
            return;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    while (term_count < n) compute_next_term();
    return term_cache[n - 1];
}
