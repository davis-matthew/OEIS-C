/*
 * A003078  Number of letters in n (in Danish).
 * https://oeis.org/A003078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Danish name of n, built from the
 *            word lengths of the units, teens and tens (halvfjerds = 10,
 *            halvfems = 8), "og" (2), "hundrede" (8), "tusind" (6), "million"
 *            (7)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int count_under_1000(int n) {
    static const int ones[] = {0, 2, 2, 3, 4, 3, 4, 3, 4, 2};
    static const int teens[] = {2, 6, 4, 7, 7, 6, 7, 6, 5, 6};
    static const int tens[] = {0, 0, 4, 7, 5, 9, 4, 10, 4, 8};

    if (n == 0) return 0;
    if (n < 10) return ones[n];
    if (n < 20) return teens[n - 10];
    if (n < 100) {
        int q = n / 10, r = n % 10;
        return tens[q] + (r > 0 ? 2 + ones[r] : 0);
    }
    int q = n / 100, r = n % 100;
    int result = (q > 1 ? ones[q] : 0) + 8;
    if (r > 0) {
        result += 2 + count_under_1000(r);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 1000) {
        return (seq_t)count_under_1000((int)n);
    }

    if (n < 1000000) {
        int q = (int)(n / 1000);
        int r = (int)(n % 1000);

        int result = count_under_1000(q) + 6;
        if (r > 0) {
            result += 2 + count_under_1000(r);
        }

        return (seq_t)result;
    }

    int q = (int)(n / 1000000);
    int r = (int)(n % 1000000);

    int result = count_under_1000(q) + 7;
    if (r > 0) {
        result += 2;
        if (r < 1000) {
            result += count_under_1000(r);
        } else {
            int q2 = r / 1000;
            int r2 = r % 1000;
            result += count_under_1000(q2) + 6;
            if (r2 > 0) {
                result += 2 + count_under_1000(r2);
            }
        }
    }

    return (seq_t)result;
}
