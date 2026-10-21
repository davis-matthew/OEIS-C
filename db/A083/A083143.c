/*
 * A083143  Smallest n-digit palindromic multiple of prime(n), or 0 if no such
 *          number exists.
 * https://oeis.org/A083143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest n‑digit palindromic multiple of prime(n), or 0
 *            if none exists
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Prime of order n */
    seq_t prime_seq = get_prime(n);
    /* Convert to a machine integer (p <= 104729 for n <= 10000) */
    idx_t p = (idx_t)prime_seq;
    if (p == 0) return (seq_t)0;          /* safety */

    /* Length of the left part (ceil(n/2)) */
    idx_t k = (n + 1) / 2;
    /* arrays for digit handling and modular coefficients */
    uint8_t *digits = (uint8_t *)calloc((size_t)k, sizeof(uint8_t));
    idx_t *pow10_fwd  = (idx_t *)calloc((size_t)k, sizeof(idx_t));
    idx_t *pow10_rev  = (idx_t *)calloc((size_t)k, sizeof(idx_t));
    if (!digits || !pow10_fwd || !pow10_rev) {
        free(digits); free(pow10_fwd); free(pow10_rev);
        return (seq_t)0;
    }

    /* Precompute 10^i mod p for reverse direction and 10^(k-1-i) mod p for forward */
    idx_t tmp = 1 % p;                     /* pow(10,0) */
    for (idx_t i = 0; i < k; ++i) {        /* reverse powers: 10^i */
        pow10_rev[i] = tmp;
        tmp = (tmp * 10) % p;
    }
    tmp = 1 % p;
    for (idx_t i = k; i > 0; --i) {        /* forward powers: 10^(k-1-i) */
        pow10_fwd[i-1] = tmp;
        tmp = (tmp * 10) % p;
    }

    /* Initial number: 1 followed by zeros */
    digits[0] = 1;
    for (idx_t i = 1; i < k; ++i) digits[i] = 0;

    /* Initial mod values */
    idx_t P_mod  = (digits[0] * pow10_fwd[0]) % p;   /* only first digit is 1 */
    idx_t rev_mod = (digits[0] * pow10_rev[0]) % p;   /* rev(power) */
    /* Precompute 10^k mod p and 10^(k-1) mod p for the palindrome construction */
    idx_t pow10_k_mod      = (pow10_fwd[0] * 10) % p;   /* 10^k mod p */
    idx_t pow10_k_minus1_mod = pow10_fwd[0];            /* 10^(k-1) mod p */

    while (1) {
        /* Compute remainder of the candidate palindrome */
        idx_t rem;
        if ((n & 1) == 0) { /* even length */
            rem = ( (P_mod * pow10_k_mod) % p + rev_mod ) % p;
        } else {            /* odd length */
            idx_t last_digit = digits[k-1];
            idx_t tmp_term = (last_digit * pow10_rev[k-1]) % p;
            idx_t revB_mod = (rev_mod + p - tmp_term) % p;
            rem = ( (P_mod * pow10_k_minus1_mod) % p + revB_mod ) % p;
        }

        if (rem == 0) {
            /* Build the full palindrome number */
            seq_t ans = 0;
            /* forward part */
            for (idx_t i = 0; i < k; ++i) {
                ans = ans * 10 + (seq_t)digits[i];
            }
            /* reverse part */
            if ((n & 1) == 0) {                    /* even */
                for (idx_t i = k; i > 0; --i) {
                    ans = ans * 10 + (seq_t)digits[i-1];
                }
            } else {                                /* odd */
                for (idx_t i = k-1; i > 0; --i) {
                    ans = ans * 10 + (seq_t)digits[i-1];
                }
            }
            free(digits); free(pow10_fwd); free(pow10_rev);
            return ans;
        }

        /* Increment the k‑digit number (lexicographically smallest) */
        idx_t carry = 1;
        idx_t i;
        for (i = k; i > 0; --i) {
            idx_t pos = i - 1;
            idx_t old_digit = digits[pos];
            idx_t new_digit = old_digit + carry;
            if (new_digit >= 10) {
                digits[pos] = (uint8_t)(new_digit - 10);
                carry = 1;
                /* update modular values for this digit */
                idx_t old_fwd = (old_digit * pow10_fwd[pos]) % p;
                idx_t new_fwd = (digits[pos] * pow10_fwd[pos]) % p;
                P_mod = (P_mod + new_fwd + p - old_fwd) % p;
                idx_t old_rev = (old_digit * pow10_rev[pos]) % p;
                idx_t new_rev = (digits[pos] * pow10_rev[pos]) % p;
                rev_mod = (rev_mod + new_rev + p - old_rev) % p;
            } else {
                digits[pos] = (uint8_t)new_digit;
                /* update modular values for this digit */
                idx_t old_fwd = (old_digit * pow10_fwd[pos]) % p;
                idx_t new_fwd = (digits[pos] * pow10_fwd[pos]) % p;
                P_mod = (P_mod + new_fwd + p - old_fwd) % p;
                idx_t old_rev = (old_digit * pow10_rev[pos]) % p;
                idx_t new_rev = (digits[pos] * pow10_rev[pos]) % p;
                rev_mod = (rev_mod + new_rev + p - old_rev) % p;
                carry = 0;
                break;
            }
        }
        if (carry) {          /* overflow: all digits were 9 */
            free(digits); free(pow10_fwd); free(pow10_rev);
            return (seq_t)0;
        }
    }
}
