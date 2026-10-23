/*
 * A043475  Numbers having three 5's in base 9.
 * https://oeis.org/A043475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly three 5’s in base 9, listed in increasing order */

static inline seq_t pow8(unsigned int e) {
    seq_t r = 1;
    while (e--) r *= (seq_t)8;
    return r;
}

static inline seq_t comb_small(unsigned int n, unsigned int k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (unsigned int i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* count of base‑9 numbers of length L with exactly three digits equal to 5 */
static inline seq_t count_len(unsigned int L) {
    seq_t cnt = 0;
    if (L >= 3) {
        /* most significant digit is 5 */
        cnt += comb_small(L - 1, 2) * pow8(L - 3);
    }
    if (L >= 4) {
        /* most significant digit is not 5 (7 possible non‑zero digits) */
        cnt += 7 * comb_small(L - 1, 3) * pow8(L - 4);
    }
    return cnt;
}

seq_t a(idx_t n) { /* n ≥ 1 */
    /* find length L of the n‑th number */
    unsigned int L = 3;
    seq_t cumulative = 0;
    while (1) {
        seq_t cnt = count_len(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }

    /* offset within length L numbers (0‑based) */
    idx_t offset = (idx_t)(n - cumulative - 1);

    unsigned int fives = 0;
    uint8_t digits[20]; /* length will never exceed 20 */

    for (unsigned int pos = 0; pos < L; ++pos) {
        unsigned int start = (pos == 0) ? 1 : 0;
        for (unsigned int d = start; d <= 8; ++d) {
            unsigned int nf = fives + (d == 5 ? 1 : 0);
            if (nf > 3) continue;

            unsigned int rem = L - pos - 1;
            unsigned int need = 3 - nf;
            if (need > rem) continue;

            seq_t count =
                comb_small(rem, need) * pow8(rem - need);

            if ((idx_t)count <= offset) {
                offset -= (idx_t)count;
                continue;
            }

            /* choose this digit */
            digits[pos] = (uint8_t)d;
            fives = nf;
            break;
        }
    }

    /* convert base‑9 digits to decimal */
    seq_t value = 0;
    for (unsigned int pos = 0; pos < L; ++pos)
        value = value * (seq_t)9 + (seq_t)digits[pos];

    return value;
}
