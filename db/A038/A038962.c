/*
 * A038962  Primes p such that 77 is not a square mod p.
 * https://oeis.org/A038962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/jacobi_symbol.h"

/* Primes p such that 77 is a quadratic non‑residue mod p (p = 2 is
   included). */
static seq_t* seq_arr = NULL;     /* computed values of the sequence   */
static size_t seq_len = 0;        /* number of values stored           */
static size_t seq_cap = 0;        /* capacity of seq_arr               */
static uint64_t next_cand = 2;    /* next integer to test for primality */

static void ensure_terms(idx_t n)
{
    if (n <= seq_len) return;

    while (seq_len < n) {
        bool is_prime = false;
        if (next_cand == 2) {
            is_prime = true;
        } else {
            /* Only odd numbers >2 can be prime. */
            is_prime = isPrimeTrial((seq_t)next_cand);
        }

        if (is_prime) {
            bool include = false;
            if (next_cand == 2) {
                include = true;                  /* 2 is always listed   */
            } else {
                int j = jacobi_symbol(77ULL, next_cand);
                if (j == -1) include = true;     /* non‑residue case    */
            }

            if (include) {
                if (seq_len >= seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 8;
                    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = (seq_t)next_cand;
            }
        }

        /* Advance to the next candidate integer. */
        if (next_cand == 2) {
            next_cand = 3;
        } else {
            next_cand += 2;   /* skip even numbers */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but harmless */
    ensure_terms(n);
    return seq_arr[n - 1];
}
