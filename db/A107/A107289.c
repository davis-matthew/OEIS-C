/*
 * A107289  Numbers k such that the sum of digits of k^2 is a prime.
 * https://oeis.org/A107289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107289: Numbers k such that the sum of digits of k^2 is a prime.
   The function generates the sequence by testing successive k, squaring,
   summing the digits of k^2, checking primality of that sum, and collecting
   those k for which the sum is prime.  Results are memoised so that calls
   with arbitrary n are answered in O(n) time overall. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for memoisation */
static idx_t          memo_size = 0;   /* number of terms already computed    */
static idx_t          memo_cap  = 0;   /* capacity of the memo array           */
static seq_t          *memo_terms = NULL;  /* array of terms                       */
static idx_t          next_k = 1;      /* next integer to test                 */

/* Very small trial‑division primality test (sum of digits never exceeds 9·digits). */
static bool is_prime_small(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* out of range – return 0 for safety */

    /* Produce terms until we have at least n stored. */
    while (memo_size < n) {
        seq_t k   = (seq_t)next_k;      /* candidate k */
        seq_t sq  = k * k;              /* k² */

        /* Compute sum of decimal digits of k².  We can use the 64‑bit integer
           representation of k directly; k² for the required range fits in
           64 bits so the cast is safe. */
        uint64_t k64 = (uint64_t)k;
        uint64_t sum = 0;
        uint64_t temp = k64 * k64;      /* k² in 64 bits for digit sum */
        while (temp > 0) {
            sum += temp % 10;
            temp /= 10;
        }

        /* If the digit sum is prime we store this k as a term. */
        if (is_prime_small(sum)) {
            if (memo_size == memo_cap) {
                memo_cap = memo_cap ? memo_cap * 2 : 16;
                memo_terms = (seq_t*)realloc(memo_terms, memo_cap * sizeof(seq_t));
            }
            memo_terms[memo_size++] = k;
        }
        ++next_k;
    }

    return memo_terms[n - 1];
}
