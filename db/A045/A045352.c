/*
 * A045352  Primes congruent to {1, 2, 5, 7} mod 8.
 * https://oeis.org/A045352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 1, 2, 5, or 7 (mod 8); that
 *            is, all primes except those ≡ 3 (mod 8)
 * Range:     u8 n <= 39, u16 n <= 4907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Local storage for the filtered primes (the sequence) and for all primes found so far */
static seq_t *filtered = NULL;
static size_t filtered_len = 0;
static size_t filtered_cap = 0;

static seq_t *all_primes = NULL;
static size_t all_len = 0;
static size_t all_cap = 0;
static idx_t last_checked = 1;   /* numbers processed up to last_checked */

/* Helper to grow an array to at least 'need' elements.  It doubles the capacity. */
static inline void grow_seq(seq_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset is 1; n=0 is outside the sequence */

    /* Keep generating primes until we have at least n elements in 'filtered'. */
    while (filtered_len < (size_t)n) {
        last_checked++;                        /* next candidate */

        /* Skip even numbers > 2 (they cannot be prime). */
        if (last_checked > 2 && (last_checked & 1U) == 0U)
            continue;

        /* Primality test by trial division using all smaller primes found so far. */
        bool prime = true;
        for (size_t i = 0; i < all_len; ++i) {
            seq_t p = all_primes[i];
            seq_t p2 = p * p;
            if (p2 > (seq_t)last_checked)
                break;
            if ((last_checked % p) == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            /* Store the new prime in the list of all primes. */
            grow_seq(&all_primes, &all_len, &all_cap, all_len + 1);
            all_primes[all_len++] = (seq_t)last_checked;

            /* Keep the prime if it satisfies the congruence condition. */
            if ((last_checked == (idx_t)2) ||
                ((last_checked % 8ULL) == 1ULL) ||
                ((last_checked % 8ULL) == 5ULL) ||
                ((last_checked % 8ULL) == 7ULL)) {
                grow_seq(&filtered, &filtered_len, &filtered_cap, filtered_len + 1);
                filtered[filtered_len++] = (seq_t)last_checked;
            }
        }
    }

    return filtered[n - 1];
}
