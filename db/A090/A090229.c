/*
 * A090229  Differences between consecutive nonprime Fibonacci numbers
 *          (A090206).
 * https://oeis.org/A090229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Differences between consecutive nonprime Fibonacci numbers (A090229) */
/* The value a(n) is computed by generating Fibonacci numbers in order, detecting
   which Fibonacci values are nonprime (primes occur only when the index is
   prime or exactly 4), and storing the difference between each successive
   nonprime Fibonacci number.  A static dynamic array caches all computed
   differences so that repeated calls with arbitrary n have O(1) lookup
   after the necessary terms have been generated. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"        /* bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t) */

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    /* Static cache of computed differences */
    static seq_t *diff_arr   = NULL;   /* diff_arr[k] = a(k) */
    static size_t diff_len   = 0;      /* number of computed differences */
    static size_t diff_cap   = 0;      /* allocated capacity of diff_arr   */

    /* State for Fibonacci generation */
    static seq_t f_prev = (seq_t)0;    /* F_k   */
    static seq_t f_next = (seq_t)1;    /* F_{k+1} */
    static idx_t  cur_idx = 0;         /* current index k of f_prev */

    /* For tracking the last nonprime Fibonacci number seen */
    static seq_t last_nonprime   = 0;
    static bool   has_last_nonprime = false;

    /* Generate differences until we have at least n+1 of them */
    while (diff_len <= n) {
        seq_t val = f_prev;           /* current Fibonacci value */

        /* Determine whether val is a nonprime Fibonacci number. */
        bool nonprime = false;
        if (cur_idx <= 2) {                     /* F0, F1, F2 are 0,1,1 */
            nonprime = true;
        } else if (cur_idx == 4) {              /* F4 = 3 is prime (exception) */
            nonprime = false;
        } else if (is_prime(cur_idx)) {         /* prime index – test the value */
            if (val == (seq_t)0 || val == (seq_t)1) {
                nonprime = true;                /* 0 or 1 are not prime */
            } else {
                nonprime = !is_prime_big(val);  /* nonprime iff not prime */
            }
        } else {                                /* composite index >4 → composite F_n */
            nonprime = true;
        }

        /* If this Fibonacci number is nonprime, record the difference */
        if (nonprime) {
            if (!has_last_nonprime) {
                last_nonprime   = val;
                has_last_nonprime = true;
            } else {
                /* Ensure the array has space for a new value */
                if (diff_len == diff_cap) {
                    size_t new_cap = diff_cap == 0 ? 4 : diff_cap * 2;
                    seq_t *tmp = (diff_arr == NULL)
                                 ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                 : (seq_t *)realloc(diff_arr,
                                                    new_cap * sizeof(seq_t));
                    if (tmp == NULL) /* allocation failure – abort */
                        return (seq_t)0;
                    diff_arr = tmp;
                    diff_cap = new_cap;
                }
                diff_arr[diff_len] = val - last_nonprime;
                diff_len++;
                last_nonprime = val;
            }
        }

        /* Step to the next Fibonacci number */
        seq_t next = f_prev + f_next;
        f_prev    = f_next;
        f_next    = next;
        cur_idx++;                            /* advance index counter */
    }

    return diff_arr[n];
}
