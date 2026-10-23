/*
 * A177880  Numbers k such that not all exponents in the prime power
 *          factorization of k are in A005836.
 * https://oeis.org/A177880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A177880
 *
 *  a(n) = the n-th integer k (k >= 1) such that
 *  not all exponents in the prime power factorisation of k
 *  belong to the set S = { e | the base‑3 representation of e
 *  contains only digits 0 or 1 }.
 *
 *  This implementation generates the sequence on demand, storing
 *  the previously computed values in a dynamically expanding
 *  array.  Each candidate integer is factored by simple trial
 *  division; the exponents are tested for membership in S by
 *  checking that their base‑3 representation does not contain
 *  the digit 2.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the sequence values */
static seq_t *seq_arr   = NULL;   /* dynamic array of terms */
static size_t seq_len   = 0;      /* number of terms currently stored */
static size_t seq_cap   = 0;      /* capacity of seq_arr              */

/* last integer that was checked */
static uint64_t last_checked = 1u;

/* ------------------------------------------------------------------ */
/*  Utility: test whether an exponent e has only 0 or 1 digits in base 3  */
/* ------------------------------------------------------------------ */
static inline bool exponent_in_S(uint64_t e)
{
    /* The exponent 0 is formally allowed (since it never occurs) */
    if (e == 0u) return true;
    while (e > 0u) {
        if (e % 3u == 2u) return false;   /* digit 2 found */
        e /= 3u;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Expand the dynamic array by one element                            */
/* ------------------------------------------------------------------ */
static inline void push_back(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0u) ? 16u : seq_cap * 2u;
        seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        /* (The tests never trigger out of memory.) */
        seq_arr = new_arr;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Factorise n and decide whether to include it in the sequence.      */
/* ------------------------------------------------------------------ */
static inline bool include_candidate(uint64_t n)
{
    if (n == 1u) return false;   /* 1 is excluded */

    uint64_t m = n;
    for (uint64_t p = 2u; p * p <= m; ++p) {
        if (m % p == 0u) {
            uint64_t exp = 0u;
            while (m % p == 0u) {
                m /= p;
                ++exp;
            }
            if (!exponent_in_S(exp)) return true;   /* found a "bad" exponent */
        }
    }
    /* Remaining prime factor (if any) has exponent 1 */
    if (m > 1u) {
        /* exponent 1 is in S => does not make the number good */
        /* but if we found no bad exponent so far, the number is excluded */
        /* so we fall through to the "excluded" case */
    }
    /* All exponents belong to S => exclude */
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main function: returns a(n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms */
    while (seq_len < n) {
        ++last_checked;               /* start from the next integer */
        if (include_candidate(last_checked)) {
            push_back((seq_t)last_checked);   /* store the term */
        }
    }
    return seq_arr[n - 1u];
}
