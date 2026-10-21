/*
 * A075928  List of codewords in binary lexicode with Hamming distance 4
 *          written as decimal numbers.
 * https://oeis.org/A075928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n-th term of the binary lexicode with Hamming
 * distance 4.  The term is obtained by greedily scanning all non‑negative
 * integers and accepting a candidate iff it differs in at least four bit
 * positions from every previously accepted codeword. */
#include <stdlib.h>
#include <stdbool.h>

/* popcount for seq_t – works for any integer type supporting >> & 1
 * (seq_t may be 8, 16, 32, 64 or 128 bits, or an arbitrary‑precision
 * class that is trivially copyable). */
static inline int popcount_seq_t(seq_t x)
{
    int c = 0;
    while (x) {
        c += (int)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Append a value to a dynamically sized array.  The array is grown as
 * necessary (doubling each time).  Works for any seq_t that can be
 * moved with memcpy (including arbitrary‑precision types that are
 * trivially copyable). */
static void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort(); /* abort on allocation failure */
        *arr = tmp;
        *cap = newcap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Global cache of terms already computed (thread‑unsafe, but the
 * exercise specifies a single process). */
static seq_t *cache      = NULL;
static size_t cachelen   = 0;
static size_t cachecap   = 0;
static idx_t  last_checked = 0;      /* last integer candidate examined */

/* Return the n-th lexicode term (index starts at 0). */
seq_t a(idx_t n)
{
    /* Ensure cache contains at least n+1 terms. */
    while (cachelen <= (size_t)n) {
        seq_t cand = (seq_t)last_checked;   /* current candidate */
        bool ok = true;
        for (size_t i = 0; i < cachelen; ++i) {
            int dist = popcount_seq_t(cand ^ cache[i]);
            if (dist < 4) { ok = false; break; }
        }
        if (ok) {
            append_seq(&cache, &cachelen, &cachecap, cand);
        }
        last_checked++;   /* advance to next integer */
    }
    return cache[(size_t)n];
}
