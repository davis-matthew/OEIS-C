/*
 * A132597  Let S be the concatenation of natural numbers in ternary
 *          representation: a(n) = position of first occurrence of n in S,
 *          then this occurrence in S is replaced by a(n) in ternary.
 * https://oeis.org/A132597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 8488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   to_ternary_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A132597:
   a(n) = position of the first occurrence of the ternary representation of n
   in the dynamic string S.  After that occurrence is found, the substring
   in S is replaced by the ternary representation of a(n).  The string S
   is expanded lazily by appending successive natural numbers in base‑3
   as needed, starting from 1.  The function works for all n ≥ 1,
   uses internal memoisation, and is valid C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/to_ternary_seq_t.h"

/* convert a seq_t value to a base‑3 string into out[0..len-1]       */
/* out must have space for at most 128 chars, len receives the       */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Constants: offset is 1 → a(1) is first term */
    if (n == 0)
        return (seq_t)0;

    /* ---------- Static state ---------- */
    static seq_t *values  = NULL;   /* 1‑based, values[0] unused */
    static size_t val_cap = 0;
    static idx_t  lastComputed = 0;

    static char *S          = NULL; /* dynamic ternary string */
    static size_t S_len     = 0;
    static size_t S_cap     = 0;
    static idx_t next_extend = 1;    /* next natural number to append */

    /* Ensure values array capacity */
    if (!values) {
        val_cap = 16;
        values = (seq_t*)calloc(val_cap, sizeof(seq_t));
    }
    if ((size_t)n >= val_cap) {
        size_t newcap = val_cap ? val_cap * 2 : 16;
        while (newcap <= (size_t)n) newcap *= 2;
        values = (seq_t*)realloc(values, newcap * sizeof(seq_t));
        val_cap = newcap;
    }

    /* Return if already computed */
    if (n <= lastComputed)
        return values[n];

    /* Compute from lastComputed+1 up to n */
    for (idx_t k = lastComputed + 1; k <= n; ++k) {
        /* Pattern: ternary of k */
        char pat[128];
        size_t pat_len;
        to_ternary_seq_t((seq_t)k, pat, &pat_len);

        /* Search for first occurrence, expanding S as needed */
        size_t pos = (size_t)-1; /* sentinel for "not found" */
        while (true) {
            if (pat_len <= S_len) {
                /* linear search */
                for (size_t i = 0; i + pat_len <= S_len; ++i) {
                    if (S[i] == pat[0]) {
                        bool ok = true;
                        for (size_t j = 1; j < pat_len; ++j) {
                            if (S[i + j] != pat[j]) {
                                ok = false;
                                break;
                            }
                        }
                        if (ok) {
                            pos = i;
                            break;
                        }
                    }
                }
            }
            if (pos != (size_t)-1)
                break; /* found */

            /* Append next natural number in S */
            char num[128];
            size_t num_len;
            to_ternary_seq_t((seq_t)next_extend, num, &num_len);

            /* Ensure capacity */
            if (S_len + num_len > S_cap) {
                size_t newcap = S_cap ? S_cap * 2 : 16;
                while (newcap < S_len + num_len) newcap <<= 1;
                S = (char*)realloc(S, newcap);
                S_cap = newcap;
            }
            memcpy(S + S_len, num, num_len);
            S_len += num_len;
            ++next_extend;
        }

        /* Store a(k) */
        seq_t a_val = (seq_t)(pos + 1); /* 1‑based position */
        values[k] = a_val;

        /* Replacement: ternary of a_val */
        char repl[128];
        size_t repl_len;
        to_ternary_seq_t(a_val, repl, &repl_len);

        if (repl_len == pat_len) {
            memcpy(S + pos, repl, repl_len);
        } else {
            /* Shift S to accommodate new length */
            long long shift = (long long)repl_len - (long long)pat_len; /* may be negative */
            size_t tail_len = S_len - (pos + pat_len);

            if (shift > 0) {
                /* Grow S if needed */
                if (S_len + (size_t)shift > S_cap) {
                    size_t newcap = S_cap ? S_cap * 2 : 16;
                    while (newcap < S_len + (size_t)shift) newcap <<= 1;
                    S = (char*)realloc(S, newcap);
                    S_cap = newcap;
                }
                /* Move tail forward */
                memmove(S + pos + repl_len, S + pos + pat_len, tail_len);
                S_len += (size_t)shift;
            } else if (shift < 0) {
                /* Move tail backward */
                memmove(S + pos + repl_len, S + pos + pat_len, tail_len);
                S_len -= (size_t)(-shift);
            }
            memcpy(S + pos, repl, repl_len);
        }
    }

    lastComputed = n;
    return values[n];
}
