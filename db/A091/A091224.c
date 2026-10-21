/*
 * A091224  Differences between consecutive irreducible GF(2)[X]-polynomials,
 *          divided by 2.
 * https://oeis.org/A091224
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+1} - p_n)/2 , where p_k is the k‑th irreducible,
 *            monic, polynomial over GF(2), ordered by its integer value
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the degree of a polynomial represented as an integer. */
static inline int poly_deg(uint64_t p) {
    /* p > 0 */
    return 63 - __builtin_clzll(p);
}

/* Compute f mod g for polynomials over GF(2).  Both f and g are integer
   representations; g has leading coefficient 1. */
static inline uint64_t poly_mod(uint64_t f, uint64_t g) {
    int deg_g = poly_deg(g);
    while (f) {
        int deg_f = poly_deg(f);
        if (deg_f < deg_g) break;
        f ^= g << (deg_f - deg_g);
    }
    return f;                 /* remainder */
}

/* Main function computing the sequence. */
seq_t a(idx_t n) {
    /* Static cache of the integer representations of irreducible
       polynomials. */
    static uint64_t *irreducibles = NULL;   /* array of p_k values */
    static size_t irreducible_count = 0;      /* number stored */
    static size_t irreducible_cap   = 0;      /* current capacity */
    static uint64_t next_candidate = 2;      /* integer to test next */

    /* Ensure we have at least n+1 irreducibles. */
    while (irreducible_count < (size_t)(n + 1)) {
        /* Find the next irreducible polynomial. */
        for (;;) {
            /* Ignore the constant polynomial 1. */
            if (next_candidate <= 1) { ++next_candidate; continue; }

            uint64_t cand = next_candidate;
            int deg_cand = poly_deg(cand);
            bool irreducible = true;

            /* Test divisibility by all previously found irreducibles
               whose degree ≤ deg_cand/2. */
            for (size_t i = 0; i < irreducible_count; ++i) {
                uint64_t g = irreducibles[i];
                int deg_g = poly_deg(g);
                if (deg_g > deg_cand / 2) break;
                if (poly_mod(cand, g) == 0) {
                    irreducible = false;
                    break;
                }
            }

            ++next_candidate; /* advance for next trial */

            if (irreducible) {
                /* Store the new irreducible polynomial. */
                if (irreducible_count >= irreducible_cap) {
                    size_t new_cap = irreducible_cap ? irreducible_cap * 2 : 64;
                    uint64_t *new_arr = (uint64_t*)realloc(irreducibles,
                                                          new_cap * sizeof(uint64_t));
                    irreducibles = new_arr;
                    irreducible_cap = new_cap;
                }
                irreducibles[irreducible_count++] = cand;
                break;          /* exit the inner for-loop, continue outer while */
            }
        }
    }

    /* Compute a(n) = (p_{n+1} - p_n)/2. */
    uint64_t diff = irreducibles[n] - irreducibles[n - 1];
    return (seq_t)(diff >> 1);
}
