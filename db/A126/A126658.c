/*
 * A126658  Prime numbers that are the sum of three distinct positive eighth
 *          powers.
 * https://oeis.org/A126658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime that can be expressed as the sum of three
 *            distinct positive eighth powers
 * Range:     u8 none, u16 none, u32 none, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   append_candidate, cmp_seq_t, is_prime_big, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
/* Utility headers */
#include "../utils/append_candidate.h"
#include "../utils/seq_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"
#include "../utils/cmp_seq_t.h"

seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order calls. */
    static idx_t   max_c = 0;             /* largest c processed */
    static seq_t *powers = NULL;          /* powers[1..max_c] = i^8   */
    static size_t powers_cap = 0;         /* capacity of powers       */
    static seq_t *prime_array = NULL;    /* all primes found (maybe unsorted) */
    static size_t prime_len = 0;          /* current number of elements    */
    static size_t prime_cap = 0;          /* capacity of prime_array       */

    /* --------------------------------------------------------------------
       Generate new triples a<b<c until we have at least n primes (unsorted).
       -------------------------------------------------------------------- */
    while (prime_len < n) {
        /* Increase c and compute its eighth power */
        ++max_c;
        if (max_c >= powers_cap) {
            size_t new_cap = (powers_cap == 0) ? 4 : powers_cap * 2;
            if (new_cap < max_c + 1) new_cap = max_c + 1;
            powers = seq_realloc(powers, powers_cap, new_cap);
            powers_cap = new_cap;
        }
        powers[max_c] = seq_pow((seq_t)max_c, (idx_t)8);

        /* Enumerate all distinct pairs (a,b) with a<b<c */
        for (idx_t a = 1; a + 2 <= max_c; ++a) {
            seq_t pa = powers[a];
            for (idx_t b = a + 1; b + 1 <= max_c; ++b) {
                seq_t sum = pa + powers[b] + powers[max_c];
                if (is_prime_big(sum))
                    append_candidate(&prime_array, &prime_len, &prime_cap, sum);
            }
        }
    }

    /* --------------------------------------------------------------------
       After generating enough primes (unsorted) we sort and dedupe.
       This may be repeated if duplicates were removed and more primes
       are still needed.
       -------------------------------------------------------------------- */
    while (true) {
        /* Sort the array and remove duplicates */
        qsort(prime_array, prime_len, sizeof(seq_t), cmp_seq_t);

        size_t uniq = 0;
        for (size_t i = 0; i < prime_len; ++i) {
            if (uniq == 0 || cmp_seq_t(&prime_array[i],
                                      &prime_array[uniq - 1]) != 0) {
                prime_array[uniq++] = prime_array[i];
            }
        }
        prime_len = uniq;   /* now prime_len is the number of unique primes */

        if (prime_len >= n)
            break;          /* enough primes have been found */
        /* Otherwise, generate more triples and repeat */
        while (prime_len < n) {
            ++max_c;
            if (max_c >= powers_cap) {
                size_t new_cap = (powers_cap == 0) ? 4 : powers_cap * 2;
                if (new_cap < max_c + 1) new_cap = max_c + 1;
                powers = seq_realloc(powers, powers_cap, new_cap);
                powers_cap = new_cap;
            }
            powers[max_c] = seq_pow((seq_t)max_c, (idx_t)8);

            for (idx_t a = 1; a + 2 <= max_c; ++a) {
                seq_t pa = powers[a];
                for (idx_t b = a + 1; b + 1 <= max_c; ++b) {
                    seq_t sum = pa + powers[b] + powers[max_c];
                    if (is_prime_big(sum))
                        append_candidate(&prime_array, &prime_len, &prime_cap, sum);
                }
            }
        }
    }

    return prime_array[n - 1];
}
