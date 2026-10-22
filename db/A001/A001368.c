/*
 * A001368  Number of letters in n (in Irish Gaelic).
 * https://oeis.org/A001368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  word, nonn
 * Formula:   a(n) = number of letters in the Irish Gaelic name of n, with
 *            lenition h's and the particle "a" dropped (decimal tens: fice,
 *            tríoca, ceatraca, caoga, seasca, seactó, octó, nóca), built
 *            recursively from the words for units, tens, céad, míle, milliún,
 *            billiún, trilliún
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int units[] = {4, 3, 2, 3, 7, 4, 2, 5, 3, 4};
    static const int tens[] = {0, 0, 4, 6, 8, 5, 6, 6, 4, 4};

    if (n < 10) {
        return (seq_t)units[n];
    }

    if (n == 10) {
        return 4;
    }

    if (n < 20) {
        return (seq_t)(units[n - 10] + 4);
    }

    if (n < 100) {
        seq_t result = (seq_t)tens[n / 10];
        if (n % 10 > 0) {
            result += (seq_t)units[n % 10];
        }
        return result;
    }

    if (n < 1000) {
        idx_t h = n / 100;
        seq_t result = (h == 1) ? (seq_t)4 : (seq_t)(units[h] + 4);
        if (n % 100 > 0) {
            result += a(n % 100);
        }
        return result;
    }

    if (n < 1000000) {
        idx_t k = n / 1000;
        seq_t result = (k == 1) ? (seq_t)4 : (seq_t)(a(k) + 4);
        if (n % 1000 > 0) {
            result += a(n % 1000);
        }
        return result;
    }

    if (n < 1000000000) {
        idx_t m = n / 1000000;
        seq_t result = (m == 1) ? (seq_t)6 : (seq_t)(a(m) + 6);
        if (n % 1000000 > 0) {
            result += a(n % 1000000);
        }
        return result;
    }

    if (n < 1000000000000ULL) {
        idx_t b = n / 1000000000;
        seq_t result = (b == 1) ? (seq_t)6 : (seq_t)(a(b) + 6);
        if (n % 1000000000 > 0) {
            result += a(n % 1000000000);
        }
        return result;
    }

    idx_t t = n / 1000000000000ULL;
    seq_t result = (t == 1) ? (seq_t)7 : (seq_t)(a(t) + 7);
    if (n % 1000000000000ULL > 0) {
        result += a(n % 1000000000000ULL);
    }
    return result;
}
