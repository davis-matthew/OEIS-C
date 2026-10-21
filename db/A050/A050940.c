/*
 * A050940  Numbers that are not the sum (of a nonempty sequence) of
 *          consecutive primes.
 * https://oeis.org/A050940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 8704, u32 n <= 8704, u64 n <= 8704,
 *            u128 n <= 8704, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A050940 – Numbers that are not the sum of a non‑empty sequence
   of consecutive primes.
   The function a(n) returns the n‑th such number (1‑based index).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*  Static data                                                                */
static seq_t *seq_vals     = NULL;   /* array of missing numbers */
static size_t  seq_len     = 0;      /* number of missing numbers computed */

/* Current sieve limit.  Larger limits are needed for larger n.               */
static size_t  sieve_lim   = 0;      /* current maximum number that has been sieved */

/* -------------------------------------------------------------------------- */
/*  Utility: dynamic reallocation of an array of seq_t                        */
static void realloc_seq_vals(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    seq_vals = tmp;
}

/* -------------------------------------------------------------------------- */
/*  Prime sieve up to 'limit'.  The primes are stored in a dynamically sized
   array passed back via 'primes_out' and the count via 'cnt_out'.          */
static void sieve_primes(size_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) abort();

    size_t prime_cap = 1000;
    uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) abort();

    size_t primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);
    *primes_out = primes;
    *cnt_out    = primes_cnt;
}

/* -------------------------------------------------------------------------- */
/*  Recompute the sequence up to a new sieve limit, filling seq_vals and
   seq_len.  The function returns the new length.                            */
static void recompute_up_to(size_t new_limit)
{
    /* Sieve primes up to new_limit */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    sieve_primes(new_limit, &primes, &primes_cnt);

    /* Mark all sums of consecutive primes */
    bool *rep_flag = (bool *)calloc(new_limit + 1, sizeof(bool));
    if (!rep_flag) abort();

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t sum = 0;
        for (size_t j = i; j < primes_cnt; ++j) {
            sum += primes[j];
            if (sum > new_limit) break;
            rep_flag[sum] = true;
        }
    }

    /* Build the array of numbers NOT representable as sums of consecutive primes */
    size_t new_cap = new_limit + 1;
    seq_t *new_vals = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_vals) abort();

    size_t new_len = 0;
    for (size_t v = 0; v <= new_limit; ++v) {
        if (!rep_flag[v]) {
            new_vals[new_len++] = (seq_t)v;
        }
    }

    /* Replace old data */
    free(seq_vals);
    free(rep_flag);
    free(primes);

    seq_vals = new_vals;
    seq_len  = new_len;
    sieve_lim = new_limit;
}

/* -------------------------------------------------------------------------- */
/*  The exported function required by the framework.                          */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* avoid illegal index */

    /* Ensure we have at least n terms.  Increase the sieve limit if needed. */
    while (seq_len < (size_t)n) {
        size_t new_limit = sieve_lim == 0 ? 20000 : sieve_lim * 2;
        recompute_up_to(new_limit);
    }

    return seq_vals[(size_t)n - 1];
}
