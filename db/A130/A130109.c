/*
 * A130109  Rearrangement of natural numbers such that each two terms sum up
 *          to a perfect square.
 * https://oeis.org/A130109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A130109 – Rearrangement of natural numbers in which every
   consecutive pair sums to a perfect square.
   Construction: greedy – for each square s², pair the smallest unused
   numbers a,b with a+b = s².  The sequence is stored in a static cache
   and any call a(n) retrieves the n‑th element in O(1) after the
   prefixes have been generated. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Bitset used to remember which natural numbers have already appeared. */
static uint64_t *used_bits = NULL;
static size_t used_words = 0;   /* number of 64‑bit words */

/* Ensure the bitset can hold the bit for index `i`. */
static void
ensure_used_bits(idx_t i)
{
    idx_t word = i >> 6;
    if ((size_t)word < used_words)
        return;
    size_t new_words = used_words ? used_words << 1 : 1;
    while ((size_t)word >= new_words)
        new_words <<= 1;
    uint64_t *tmp =
        (uint64_t *)realloc(used_bits, new_words * sizeof(uint64_t));
    for (size_t j = used_words; j < new_words; ++j)
        tmp[j] = 0;
    used_bits = tmp;
    used_words = new_words;
}

/* Test whether number `i` has been used. */
static bool
is_used(idx_t i)
{
    idx_t word = i >> 6;
    if ((size_t)word >= used_words)
        return false;
    return ((used_bits[word] >> (i & 63)) & 1ULL) != 0;
}

/* Mark number `i` as used. */
static void
set_used(idx_t i)
{
    ensure_used_bits(i);
    used_bits[i >> 6] |= 1ULL << (i & 63);
}

/* Static cache for the sequence. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* State used while generating the permutation. */
static idx_t smallest_unused = 1;   /* smallest unused number so far */
static idx_t next_root      = 2;    /* current square root */
static idx_t next_square    = 4;    /* current square (root²) */

/* Ensure that the cache contains at least `need` terms. */
static void
ensure_cache(idx_t need)
{
    if (seq_len >= (size_t)need)
        return;

    /* Grow the cache if necessary. */
    if (seq_cap < (size_t)need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1;
        if (new_cap < (size_t)need)
            new_cap = need;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }

    /* Generate terms until we have enough. */
    while (seq_len < (size_t)need) {
        /* Search for a valid pair for the current square. */
        bool found = false;
        idx_t a_candidate = smallest_unused;
        while (a_candidate < next_square) {
            if (!is_used(a_candidate)) {
                idx_t b_candidate = next_square - a_candidate;
                if (b_candidate != a_candidate && b_candidate > 0 &&
                    !is_used(b_candidate)) {
                    found = true;
                    a_candidate = a_candidate; /* use this pair */
                    break;
                }
            }
            ++a_candidate;
        }

        if (!found) {
            /* No pair for this square – advance to the next square. */
            ++next_root;
            next_square = next_root * next_root;
            smallest_unused = 1;
            while (is_used(smallest_unused))
                ++smallest_unused;
            continue;
        }

        idx_t a = a_candidate;
        idx_t b = next_square - a;
        set_used(a);
        set_used(b);

        /* Ensure capacity for the two new terms. */
        if (seq_len + 2 > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 1;
            while (new_cap < seq_len + 2)
                new_cap <<= 1;
            seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cap   = new_cap;
        }

        seq_cache[seq_len++] = (seq_t)a;
        seq_cache[seq_len++] = (seq_t)b;

        /* Advance smallest_unused to the next unused number. */
        while (is_used(smallest_unused))
            ++smallest_unused;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, but safe */
    ensure_cache(n);
    return seq_cache[(size_t)n - 1];
}
