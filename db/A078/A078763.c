/*
 * A078763  List primes between (2n-1)^2 and (2n)^2.
 * https://oeis.org/A078763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 3292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* List primes between (2n-1)^2 and (2n)^2, concatenated over successive n,
   offset 1.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit numbers.
   This suffices for the values that appear when n ≤ 10 000. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Sequence storage.  seq_array holds the primes in order.  */
static seq_t *seq_array = NULL;      /* allocated array of seq_t      */
static size_t seq_cap  = 0;          /* allocated capacity            */
static idx_t   seq_len  = 0;          /* number of terms already stored */
static idx_t   last_k   = 0;          /* last interval processed       */

/* Appends a new term to the sequence array, growing it if necessary. */
static void append_term(seq_t value)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation failure */
        seq_array = tmp;
        seq_cap   = new_cap;
    }
    seq_array[seq_len++] = value;
}

/* Generates terms until at least 'target' terms are available. */
static void ensure_sequence_upto(idx_t target)
{
    while (seq_len < target) {
        ++last_k;                                  /* next interval k   */
        uint64_t low  = ((uint64_t)(2 * last_k - 1) *
                         (uint64_t)(2 * last_k - 1)) + 1;
        uint64_t high = ((uint64_t)(2 * last_k) *
                         (uint64_t)(2 * last_k));

        for (uint64_t x = low; x <= high; ++x) {
            if (is_prime_uint64(x)) {
                append_term((seq_t)x);
            }
        }
    }
}

/* Public function: returns the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* defined as 0 for offset safety  */
    ensure_sequence_upto(n);
    return seq_array[n - 1];
}
