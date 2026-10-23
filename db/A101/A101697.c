/*
 * A101697  Numbers n such that phi(n) is equal to d_1!+d_2!+...+d_k!, where
 *          d_1 d_2 ... d_k is the decimal expansion of n.
 * https://oeis.org/A101697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101697: numbers n such that φ(n) = d1!+d2!+…+dk!
   where d1…dk are the decimal digits of n.  The routine builds a
   linear‑sieve for φ up to an expanding limit and stores each found
   term in a dynamically growing array, returning the n‑th term on
   demand. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* factorial values 0! … 9! as seq_t constants (seq_t is defined by the build) */
static const seq_t fact[10] = {
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,
    (seq_t)24, (seq_t)120, (seq_t)720, (seq_t)5040,
    (seq_t)40320, (seq_t)362880
};

/* sum of factorials of the decimal digits of x (x fits in uint32_t) */
static seq_t digit_fact_sum(uint32_t x)
{
    seq_t s = 0;
    while (x > 0) {
        int d = x % 10;
        s += fact[d];
        x /= 10;
    }
    return s;
}

/* linear sieve: compute φ[0 … limit] into *phi and return its size in *size */
static void compute_phi(uint32_t **phi, uint32_t *size, size_t limit)
{
    /* free any old array */
    if (*phi) free(*phi);
    *phi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!*phi) exit(EXIT_FAILURE);

    (*phi)[0] = 0;
    (*phi)[1] = 1;
    for (size_t i = 2; i <= limit; ++i) (*phi)[i] = (uint32_t)i;

    /* store primes seen so far */
    uint32_t primes[3000000];
    uint32_t pcnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if ((*phi)[i] == i) {          /* i is prime */
            (*phi)[i] = (uint32_t)(i - 1);
            if (pcnt < 3000000) primes[pcnt++] = (uint32_t)i;
        }
        for (uint32_t j = 0; j < pcnt; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (ip > limit) break;
            if (i % p == 0) {
                (*phi)[(uint32_t)ip] = (*phi)[i] * p;
                break;
            } else {
                (*phi)[(uint32_t)ip] = (*phi)[i] * (p - 1);
            }
        }
    }
    *size = (uint32_t)(limit + 1);
}

/* main sequence function: returns the n-th element (n >= 1). */
seq_t a(idx_t n)
{
    static bool inited = false;
    static uint32_t *phi = NULL;     /* table of φ values */
    static uint32_t phi_size = 0;    /* current table size (phi_size = limit+1) */
    static uint32_t cap = 50000;     /* capacity of result array */
    static seq_t *res = NULL;        /* stored terms */
    static uint32_t cnt = 0;         /* number of terms stored */
    static uint32_t last = 1;        /* last integer examined */

    if (!inited) {
        compute_phi(&phi, &phi_size, 1000000);
        res = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!res) exit(EXIT_FAILURE);
        res[0] = (seq_t)1;          /* a(1) = 1 */
        cnt = 1;
        last = 1;
        inited = true;
    }

    /* grow the sequence until we have at least n terms */
    while (cnt < (uint32_t)n) {
        uint32_t cand = last + 1;

        if (cand >= phi_size) {           /* need larger φ table */
            size_t new_lim = phi_size * 2;
            if (new_lim < (size_t)(cand * 2)) new_lim = (size_t)(cand * 2);
            compute_phi(&phi, &phi_size, new_lim);
        }

        seq_t phi_val   = (seq_t)phi[cand];
        seq_t digit_val = digit_fact_sum(cand);

        if (phi_val == digit_val) {
            if (cnt == cap) {             /* grow result array */
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(res, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                res = tmp;
            }
            res[cnt++] = (seq_t)cand;
        }
        last = cand;
    }

    return res[n - 1];
}
