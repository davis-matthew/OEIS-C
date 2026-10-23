/*
 * A045402  Primes congruent to {1, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A045402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045402: primes congruent to {1,3,4,5,6} mod 7
   (i.e., all primes except 2 and 7).  a(1)=3, a(2)=5, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* ---------- static state ---------- */
    static idx_t *prime_list = NULL;    /* all primes encountered */
    static size_t prime_cap   = 0;
    static size_t prime_cnt   = 0;

    static seq_t *selected     = NULL;  /* primes satisfying the residue */
    static size_t selected_cap = 0;
    static size_t selected_cnt = 0;

    static idx_t last_candidate = 5;   /* next odd number to test */

    /* ---- initialise on first call ---- */
    if (prime_cnt == 0) {
        /* start with {2,3} for primality tests */
        prime_cap = 4;
        prime_list = (idx_t*)malloc(prime_cap * sizeof(idx_t));
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_cnt = 2;

        /* first selected prime is 3 */
        selected_cap = 4;
        selected = (seq_t*)malloc(selected_cap * sizeof(seq_t));
        selected[0] = (seq_t)3;
        selected_cnt = 1;
    }

    /* ---- grow the lists as needed ---- */
    /* helper macro to resize an array of type T */
#define RESIZE_ARRAY(ptr, cap, new_cap, type)          \
    do {                                              \
        cap = new_cap;                                \
        ptr = (type*)realloc(ptr, cap * sizeof(type));\
    } while (0)

    /* ---- find enough selected primes ---- */
    while (selected_cnt < n) {
        idx_t candidate = last_candidate;

        /* primality test by trial division using stored primes */
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > candidate / p) break;   /* p^2 > candidate */
            if (candidate % p == 0) { prime = false; break; }
        }

        if (prime) {
            /* add to prime list */
            if (prime_cnt == prime_cap)
                RESIZE_ARRAY(prime_list, prime_cap, prime_cap * 2, idx_t);
            prime_list[prime_cnt++] = candidate;

            /* filter out 2, 7 and primes ≡ 2 (mod 7) */
            if (candidate != 7 && candidate % 7 != 2) {
                if (selected_cnt == selected_cap)
                    RESIZE_ARRAY(selected, selected_cap, selected_cap * 2, seq_t);
                selected[selected_cnt++] = (seq_t)candidate;
            }
        }

        last_candidate += 2;   /* only odd candidates */
    }

#undef RESIZE_ARRAY
    return selected[n - 1];
}
