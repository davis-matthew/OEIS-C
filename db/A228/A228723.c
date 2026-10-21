/*
 * A228723  Smallest number not less than n with alternating parities of
 *          digits in decimal representation.
 * https://oeis.org/A228723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m ≥ n whose decimal digits alternate in parity
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   decode_digits, digits_count_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation uses at most O(L) time per call, where L is the
   number of decimal digits of n (≤ 19 for 64‑bit n).  It works for any
   seq_t that supports the basic arithmetic and comparison operators.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/decode_digits.h"
#include "../utils/digits_count_idx.h"

/* Main function implementing the sequence */
seq_t a(idx_t n)
{
    /* n == 0 is a trivial alternating number */
    if (n == 0)
        return (seq_t)0;

    /* Decode the digits of n */
    uint8_t din[20];
    int L;
    decode_digits(n, din, &L);

    seq_t best = 0;
    bool best_set = false;

    /* Try both possible starting parities: 0 even, 1 odd */
    for (int start_parity = 0; start_parity <= 1; ++start_parity) {
        uint8_t s[20];
        bool larger = false;
        int pos = 0;
        bool fail = false;

        /* Build number of the same length L */
        for (; pos < L; ++pos) {
            int parity = start_parity ^ (pos & 1);
            int min_digit = (pos == 0) ? 1 : 0;
            int start = larger ? min_digit : (din[pos] > min_digit ? din[pos] : min_digit);

            int chosen = -1;
            for (int dig = start; dig <= 9; ++dig) {
                if ((dig & 1) == parity) {
                    chosen = dig;
                    break;
                }
            }

            if (chosen == -1) {   /* need to backtrack */
                int back = pos - 1;
                bool found = false;
                while (back >= 0 && !found) {
                    int prev_parity = start_parity ^ (back & 1);
                    int prev_digit = s[back];
                    int start_back = prev_digit + 1;
                    int min_back = (back == 0) ? 1 : 0;
                    int chosen_back = -1;
                    for (int dig = start_back; dig <= 9; ++dig) {
                        if ((dig & 1) == prev_parity && dig >= min_back) {
                            chosen_back = dig;
                            break;
                        }
                    }
                    if (chosen_back != -1) {
                        s[back] = chosen_back;
                        larger = true;   /* once we bump, the rest will be minimal */
                        for (int k = back + 1; k < L; ++k) {
                            int kparity = start_parity ^ (k & 1);
                            int min_k = (k == 0) ? 1 : 0;
                            for (int dig = min_k; dig <= 9; ++dig)
                                if ((dig & 1) == kparity) {
                                    s[k] = dig;
                                    break;
                                }
                        }
                        found = true;
                        break;
                    } else
                        --back;
                }
                if (!found) {        /* no candidate of length L, need longer */
                    fail = true;
                    break;
                } else {               /* candidate already stored in s */
                    seq_t val = 0;
                    for (int i = 0; i < L; ++i)
                        val = val * 10 + (seq_t)s[i];
                    if (!best_set || val < best) {
                        best = val;
                        best_set = true;
                    }
                    /* done for this start_parity */
                    break;
                }
            } else {                 /* choose digit normally */
                s[pos] = chosen;
                if (!larger && chosen > din[pos])
                    larger = true;
            }
        }

        /* If we completed the loop without backtracking failure,
           we have a valid candidate of length L. */
        if (!fail && pos == L) {
            seq_t val = 0;
            for (int i = 0; i < L; ++i)
                val = val * 10 + (seq_t)s[i];
            if (!best_set || val < best) {
                best = val;
                best_set = true;
            }
        }

        /* If we failed to find same-length candidate, build minimal
           longer candidate of length L+1. */
        if (fail) {
            int newL = L + 1;
            int parity = start_parity;
            /* first digit: smallest non‑zero digit of required parity */
            s[0] = (parity == 1) ? 1 : 2;   /* 1 for odd, 2 for even */
            for (int i = 1; i < newL; ++i) {
                int told = start_parity ^ (i & 1);
                s[i] = (told == 1) ? 1 : 0;  /* 1 for odd, 0 for even */
            }
            seq_t val = 0;
            for (int i = 0; i < newL; ++i)
                val = val * 10 + (seq_t)s[i];
            if (!best_set || val < best) {
                best = val;
                best_set = true;
            }
        }
    }

    return best;
}
