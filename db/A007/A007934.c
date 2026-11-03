/*
 * A007934  Some nontrivial permutation of digits is a prime.
 * https://oeis.org/A007934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search -- a(n) is the n-th number k > 10 such that some
 *            non-identity rearrangement of k's digits (leading zeros allowed)
 *            is prime; rearranging equal digits counts, so a prime with a
 *            repeated digit is a member
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9740 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static bool search_perms(const int *digits, int len, bool *used, int depth,
                         uint64_t cur, uint64_t original) {
    if (depth == len) {
        return cur != original && is_prime_u64(cur);
    }
    for (int i = 0; i < len; i++) {
        if (used[i]) continue;
        /* skip duplicate digit choices at this depth */
        bool dup = false;
        for (int j = 0; j < i; j++) {
            if (!used[j] && digits[j] == digits[i]) { dup = true; break; }
        }
        if (dup) continue;
        used[i] = true;
        if (search_perms(digits, len, used, depth + 1, cur * 10 + (uint64_t)digits[i], original)) {
            used[i] = false;
            return true;
        }
        used[i] = false;
    }
    return false;
}

static bool in_sequence(uint64_t k) {
    if (k < 11) return false;
    int digits[20];
    int len = 0;
    uint64_t t = k;
    while (t > 0) { digits[len++] = (int)(t % 10); t /= 10; }

    bool repeated = false;
    for (int i = 0; i < len && !repeated; i++)
        for (int j = i + 1; j < len; j++)
            if (digits[i] == digits[j]) { repeated = true; break; }

    /* swapping two equal digits is a nontrivial permutation yielding k itself */
    if (repeated && is_prime_u64(k)) return true;

    bool used[20];
    for (int i = 0; i < len; i++) used[i] = false;
    return search_perms(digits, len, used, 0, 0, k);
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t next_k = 11;

seq_t a(idx_t n) {
    while (nterms < n) {
        if (in_sequence(next_k)) {
            if (nterms == capterms) {
                idx_t nc = capterms ? capterms * 2 : 1024;
                terms = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
                capterms = nc;
            }
            terms[nterms++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[n - 1];
}
