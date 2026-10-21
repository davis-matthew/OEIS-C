/*
 * A052647  E.g.f. (2-2x-x^2)/((1-2x)(1-x^2)).
 * https://oeis.org/A052647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052647
   Formula: a(n) = n! * (2^n + (n mod 2))
   (i.e. a(n) = n! * (2^n + 1 if n is even, else 2^n)
   This implementation memoises factorials and powers of two.
   It is written in a portable way for both C11 and C++17, with
   explicit casts for malloc/realloc, so it works even when
   seq_t is an arbitrary‑precision integer class. */

#include <stdlib.h>

/* The function must have external linkage. */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*   Memoised factorials                                            */
    /* ------------------------------------------------------------------ */
    static seq_t *fact = NULL;       /* array of n! values                */
    static size_t fact_cap = 0;      /* current allocated capacity (number of entries) */
    static idx_t fact_last = 0;      /* highest n whose factorial is cached */

    /* ------------------------------------------------------------------ */
    /*   Memoised powers of two                                          */
    /* ------------------------------------------------------------------ */
    static seq_t *pow2 = NULL;       /* array of 2^n values              */
    static size_t pow2_cap = 0;      /* current allocated capacity         */
    static idx_t pow2_last = 0;      /* highest n whose power of two is cached */

    /* ------------------------------------------------------------------ */
    /*   Initialise caches (once)                                        */
    /* ------------------------------------------------------------------ */
    if (fact == NULL) {
        fact_cap = 1;
        fact = (seq_t*)malloc(sizeof(seq_t) * fact_cap);
        if (!fact) return (seq_t)0;                    /* allocation failed */
        fact[0] = (seq_t)1;                 /* 0! = 1                    */
        fact_last = 0;

        pow2_cap = 1;
        pow2 = (seq_t*)malloc(sizeof(seq_t) * pow2_cap);
        if (!pow2) return (seq_t)0;
        pow2[0] = (seq_t)1;                 /* 2^0 = 1                    */
        pow2_last = 0;
    }

    /* ------------------------------------------------------------------ */
    /*   Extend factorial cache if necessary                             */
    /* ------------------------------------------------------------------ */
    if (n > fact_last) {
        size_t new_cap = fact_cap;
        while (new_cap <= (size_t)n) new_cap = new_cap ? new_cap * 2 : 1;
        if (new_cap > fact_cap) {
            seq_t *tmp = (seq_t*)realloc(fact, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;
            fact = tmp;
            fact_cap = new_cap;
        }
        for (idx_t i = fact_last + 1; i <= n; ++i)
            fact[i] = fact[i - 1] * ((seq_t)i);   /* factorial recurrence     */
        fact_last = n;
    }

    /* ------------------------------------------------------------------ */
    /*   Extend power‑of‑two cache if necessary                         */
    /* ------------------------------------------------------------------ */
    if (n > pow2_last) {
        size_t new_cap = pow2_cap;
        while (new_cap <= (size_t)n) new_cap = new_cap ? new_cap * 2 : 1;
        if (new_cap > pow2_cap) {
            seq_t *tmp = (seq_t*)realloc(pow2, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;
            pow2 = tmp;
            pow2_cap = new_cap;
        }
        for (idx_t i = pow2_last + 1; i <= n; ++i)
            pow2[i] = pow2[i - 1] * ((seq_t)2);   /* 2^i recurrence           */
        pow2_last = n;
    }

    /* ------------------------------------------------------------------ */
    /*   Compute a(n)                                                    */
    /* ------------------------------------------------------------------ */
    seq_t multiplier = pow2[n] + ((n % 2 == 0) ? (seq_t)1 : (seq_t)0);
    return fact[n] * multiplier;
}
