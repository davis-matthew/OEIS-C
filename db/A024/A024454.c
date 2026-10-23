/*
 * A024454  [ (4th elementary symmetric function of P(n))/(first elementary
 *          symmetric function of P(n)) ], where P(n) = {first n+3 primes}.
 * https://oeis.org/A024454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Σ_{1 ≤ i < j < k < l ≤ n+3} p_i p_j p_k p_l) /
 *            (Σ_{i=1}^{n+3} p_i) where p_i are the first n+3 primes
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The types idx_t and seq_t are supplied by the build system;
   they need not be defined here. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* dynamic storage for primes and elementary symmetric sums */
static uint64_t *primes      = NULL;          /* primes[0 .. primes_count-1] */
static seq_t   *e1_arr      = NULL;          /* e1_arr[i] = Σ_{first i primes}        */
static seq_t   *e2_arr      = NULL;          /* e2_arr[i] = Σ of all 2‑products          */
static seq_t   *e3_arr      = NULL;          /* e3_arr[i] = Σ of all 3‑products          */
static seq_t   *e4_arr      = NULL;          /* e4_arr[i] = Σ of all 4‑products          */
static size_t  primes_count   = 0;            /* number of primes stored                */
static size_t  primes_capacity = 0;           /* allocated capacity for arrays          */

/* Ensure that at least 'need' primes (and their symmetric sums) are available. */
static void ensure_primes(size_t need)
{
    if (need <= primes_count)
        return;

    /* enlarge storage if necessary */
    if (primes_capacity < need) {
        size_t new_cap = primes_capacity ? primes_capacity * 2 : 16;
        while (new_cap < need)
            new_cap <<= 1;

        uint64_t *new_p  = (uint64_t *)realloc(primes,          new_cap * sizeof(uint64_t));
        seq_t   *new_e1  = (seq_t   *)realloc(e1_arr,        (new_cap + 1) * sizeof(seq_t));
        seq_t   *new_e2  = (seq_t   *)realloc(e2_arr,        (new_cap + 1) * sizeof(seq_t));
        seq_t   *new_e3  = (seq_t   *)realloc(e3_arr,        (new_cap + 1) * sizeof(seq_t));
        seq_t   *new_e4  = (seq_t   *)realloc(e4_arr,        (new_cap + 1) * sizeof(seq_t));

        if (!new_p || !new_e1 || !new_e2 || !new_e3 || !new_e4)
            abort();

        primes     = new_p;
        e1_arr     = new_e1;
        e2_arr     = new_e2;
        e3_arr     = new_e3;
        e4_arr     = new_e4;
        primes_capacity = new_cap;

        if (primes_count == 0) {          /* first allocation: initialize the zero slot */
            e1_arr[0] = (seq_t)0;
            e2_arr[0] = (seq_t)0;
            e3_arr[0] = (seq_t)0;
            e4_arr[0] = (seq_t)0;
        }
    }

    /* generate primes until we have 'need' of them */
    uint64_t candidate = (primes_count == 0) ? 2 : primes[primes_count - 1] + 1;

    while (primes_count < need) {
        /* skip even numbers > 2 */
        if (candidate > 2 && (candidate & 1ULL) == 0) {
            ++candidate;
            continue;
        }

        /* primality test by trial division using already known primes */
        bool prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if ( (unsigned __int128)p * p > candidate )
                break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            uint64_t p_val = candidate;
            seq_t  p_seq  = (seq_t)p_val;

            /* update elementary symmetric sums */
            seq_t new_e1 = e1_arr[primes_count] + p_seq;
            seq_t new_e2 = e2_arr[primes_count] + p_seq * e1_arr[primes_count];
            seq_t new_e3 = e3_arr[primes_count] + p_seq * e2_arr[primes_count];
            seq_t new_e4 = e4_arr[primes_count] + p_seq * e3_arr[primes_count];

            /* store new prime and updated sums */
            primes[primes_count]        = p_val;
            e1_arr[primes_count + 1]    = new_e1;
            e2_arr[primes_count + 1]    = new_e2;
            e3_arr[primes_count + 1]    = new_e3;
            e4_arr[primes_count + 1]    = new_e4;

            ++primes_count;
        }

        ++candidate;
    }
}

/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* need the first n+3 primes */
    size_t m = (size_t)(n + 3);
    ensure_primes(m);

    seq_t sum_1 = e1_arr[m];
    seq_t sum_4 = e4_arr[m];

    /* division truncates exactly because sum_4 is a multiple of sum_1 */
    return sum_4 / sum_1;
}
