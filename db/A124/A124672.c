/*
 * A124672  Oblong (promic) abundant numbers = abundant numbers of the form
 *          k(k+1).
 * https://oeis.org/A124672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124672: Oblong (promic) abundant numbers.
   Computed as the nth integer of the form k*(k+1) that is abundant,
   i.e. σ(k*(k+1)) > 2*k*(k+1) where σ denotes the sum of divisors.
   The function generates successive k values and tests the condition.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_trial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* behavior below offset not required */

    static idx_t count = 0;         /* number of terms computed so far */
    static seq_t *terms = NULL;     /* array of computed terms */
    static idx_t cap = 0;           /* capacity of terms array */
    static uint64_t last_k = 2;     /* last k processed (k starts at 3) */

    /* enlarge terms array if needed */
    if (cap < n) {
        idx_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (terms) {
            for (idx_t i = 0; i < count; ++i) tmp[i] = terms[i];
            free(terms);
        }
        terms = tmp;
        cap = new_cap;
    }

    /* generate terms until we have the requested index */
    while (count < n) {
        ++last_k;                    /* next k value, starting from 3 */
        if (last_k < 3) continue;    /* skip k=0,1,2 */

        seq_t val = (seq_t)last_k * ((seq_t)last_k + 1);
        seq_t sigma_k   = sigma_trial((uint64_t)last_k);
        seq_t sigma_k1  = sigma_trial((uint64_t)(last_k + 1));
        seq_t sigma_val = sigma_k * sigma_k1;

        if (sigma_val > val * 2) {    /* abundant test */
            terms[count++] = val;
        }
    }

    return terms[n - 1];
}
