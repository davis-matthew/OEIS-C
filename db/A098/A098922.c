/*
 * A098922  Reverse digits of largest primes, append to sequence if result is
 *          larger prime then previous one with reverse digits.
 * https://oeis.org/A098922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 20, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates A098922: 
   start with 2,3,5,7,11 and then for each prime p in increasing order 
   compute rev(p).  If rev(p) is prime and strictly larger than the 
   previously printed reverse, print rev(p).  The sequence is kept in 
   increasing order of the reverse values.  The function memoizes the
   computed values so calls with arbitrary indices are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */
#include "../utils/reverse_digits.h"   /* seq_t reverse_digits(seq_t x); */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; value for n=0 is irrelevant */

#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic storage */
    static std::vector<seq_t> seq;
    static idx_t next_candidate = 2;
    static seq_t last_rev = 0;

    if (seq.empty()) {
        seq.reserve(1024);          /* initial capacity */
        last_rev = 0;
        next_candidate = 2;
    }

    while (seq.size() < (size_t)n) {
        /* find next prime p */
        while (!is_prime_big((seq_t)next_candidate))
            ++next_candidate;

        /* candidate prime found */
        seq_t rev = reverse_digits((seq_t)next_candidate);
        if (rev > last_rev && is_prime_big(rev)) {
            seq.push_back(rev);
            last_rev = rev;
        }
        ++next_candidate;
    }
    return seq[n - 1];
#else
    /* C implementation using malloc/realloc for dynamic array */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_candidate = 2;
    static seq_t last_rev = 0;

    if (seq == NULL) {               /* first call: allocate storage */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        last_rev = 0;
        next_candidate = 2;
    }

    while (seq_len < (size_t)n) {
        /* find next prime p */
        while (!is_prime_big((seq_t)next_candidate))
            ++next_candidate;

        /* candidate prime found */
        seq_t rev = reverse_digits((seq_t)next_candidate);
        if (rev > last_rev && is_prime_big(rev)) {
            if (seq_len == seq_cap) {            /* grow storage */
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = rev;
            last_rev = rev;
        }
        ++next_candidate;
    }

    return seq[n - 1];
#endif
}
