/*
 * A238716  Run lengths of decadal prime triples.
 * https://oeis.org/A238716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate decadal prime triples (m ≡ 1 (mod 3) with at least 3 primes
   in (10m,10m+10)).  From the list of qualifying m's form maximal runs of
   consecutive m's differing by 3.  Return the lengths of these runs. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state across calls */
    static bool initialized = false;
    static idx_t current_m = 0;          /* next m to test */
    static idx_t last_qual_m = 0;        /* last qualifying m */
    static seq_t current_run_len = 0;    /* length of run being built */
    static seq_t *runs = NULL;          /* array of completed run lengths */
    static size_t runs_cap = 0, runs_len = 0;

    if (!initialized) {
        initialized = true;
        current_m = 1;          /* start with m = 1 */
        last_qual_m = 0;
        current_run_len = 0;
        runs = NULL;
        runs_cap = 0;
        runs_len = 0;
    }

    /* ensure we have at least n completed runs */
    while (runs_len < n) {
        /* skip to next candidate m ≡ 1 (mod 3) */
        while ((current_m % 3) != 1) ++current_m;

        /* count primes in the ten-block (10m,10m+10) */
        idx_t base = current_m * 10;
        idx_t prime_cnt = 0;
        for (idx_t k = 1; k <= 9; ++k) {
            if (is_prime(base + k)) {
                ++prime_cnt;
                if (prime_cnt >= 3) break;   /* early stopping */
            }
        }

        if (prime_cnt >= 3) {  /* m is a decadal prime triple decade */
            if (current_run_len == 0) {          /* start first run */
                current_run_len = 1;
                last_qual_m = current_m;
            } else if (current_m - last_qual_m == 3) { /* extend run */
                ++current_run_len;
                last_qual_m = current_m;
            } else {  /* finish previous run */
                if (runs_len == runs_cap) {
                    size_t new_cap = runs_cap ? runs_cap * 2 : 4;
                    seq_t *tmp = (seq_t*)realloc(runs, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    runs = tmp;
                    runs_cap = new_cap;
                }
                runs[runs_len++] = current_run_len;
                current_run_len = 1;
                last_qual_m = current_m;
            }
        }

        /* advance to next m */
        ++current_m;
    }

    return runs[n - 1];
}
