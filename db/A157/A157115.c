/*
 * A157115  Alternate terms of A007519, A007520, A007521, A007522.
 * https://oeis.org/A157115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th prime obtained by cycling over the four *
 *            residue classes 1, 3, 5, 7 modulo 8. * For n ≡ 1 (mod 4) the
 *            term is the k-th prime ≡ 1 (mod 8); * for n ≡ 2 (mod 4) the term
 *            is the k-th prime ≡ 3 (mod 8); * for n ≡ 3 (mod 4) the term is
 *            the k-th prime ≡ 5 (mod 8); * for n ≡ 0 (mod 4) th...
 * Range:     u8 n <= 40, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* storage for primes of each residue class modulo 8 */
static seq_t *p1 = NULL, *p3 = NULL, *p5 = NULL, *p7 = NULL;
static size_t len1 = 0, len3 = 0, len5 = 0, len7 = 0;
static size_t cap1 = 0, cap3 = 0, cap5 = 0, cap7 = 0;
static idx_t next_candidate = 2;

/* enlarge a vector to hold at least new_len elements */
static void grow_vector(seq_t **vec, size_t *len, size_t *cap, size_t new_len)
{
    if (*cap >= new_len)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*vec, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *vec = tmp;
    *cap = new_cap;
}

/* guarantee that each residue‑class vector has at least 'needed' primes */
static void ensure_primes(idx_t needed)
{
    while (len1 < needed || len3 < needed || len5 < needed || len7 < needed) {
        if (is_prime(next_candidate)) {
            int mod = (int)(next_candidate & 7);   /* next_candidate % 8 */
            seq_t val = (seq_t)next_candidate;
            switch (mod) {
                case 1:
                    grow_vector(&p1, &len1, &cap1, len1 + 1);
                    p1[len1++] = val;
                    break;
                case 3:
                    grow_vector(&p3, &len3, &cap3, len3 + 1);
                    p3[len3++] = val;
                    break;
                case 5:
                    grow_vector(&p5, &len5, &cap5, len5 + 1);
                    p5[len5++] = val;
                    break;
                case 7:
                    grow_vector(&p7, &len7, &cap7, len7 + 1);
                    p7[len7++] = val;
                    break;
                default:
                    /* ignore other residues */
                    break;
            }
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* values for n < 1 are undefined */

    /* index within each residue class */
    idx_t k = (n + 3) / 4;     /* ceil(n/4) */
    ensure_primes(k);

    int residue = (int)(n % 4);  /* 1→1, 2→3, 3→5, 0→7 */
    switch (residue) {
        case 1:
            return p1[k - 1];
        case 2:
            return p3[k - 1];
        case 3:
            return p5[k - 1];
        case 0:
            return p7[k - 1];
    }
    return 0; /* unreachable */
}
