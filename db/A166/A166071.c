/*
 * A166071  Sum of the atomic numbers in the n-th column of the Janet table of
 *          the first 120 elements.
 * https://oeis.org/A166071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   a(n) = sum of atomic numbers in the n‑th column of the Janet
 *            table of the first 120 elements (n = 1 … 32)
 * Range:     u8 n <= 20, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: full, pass, 32 terms through n = 32 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static idx_t get_col(idx_t i)
{
    /* Special case for Helium (atomic number 2): it belongs to column 32. */
    if (i == 2) return (idx_t)32;

    /* Lanthanides 57–69 → columns 1–13 */
    if (i >= 57 && i <= 69) return (idx_t)(i - 56);
    /* Actinides 89–101  → columns 1–13 */
    if (i >= 89 && i <= 101) return (idx_t)(i - 88);

    /* Yb (70) and No (102) → column 14 */
    if (i == 70 || i == 102) return (idx_t)14;

    /* Groups 3–12 in periods 4–7 occupy columns 15–24.  */
    if (i >= 21 && i <= 30)   return (idx_t)(i - 18 + 12);   /* 21–30 → 15–24 */
    if (i >= 39 && i <= 48)   return (idx_t)(i - 36 + 12);   /* 39–48 → 15–24 */
    if (i >= 71 && i <= 80)   return (idx_t)(i - 68 + 12);   /* 71–80 → 15–24 */
    if (i >= 103 && i <= 112) return (idx_t)(i - 100 + 12); /* 103–112 → 15–24 */

    /* Remaining elements belong to groups 13–18, 1 and 2
       and map to columns 25–32.  */
    switch ((int)i) {
        /* Group 13 → column 25 */
        case 5: case 13: case 31: case 49: case 81: case 113: return (idx_t)25;
        /* Group 14 → column 26 */
        case 6: case 14: case 32: case 50: case 82: case 114: return (idx_t)26;
        /* Group 15 → column 27 */
        case 7: case 15: case 33: case 51: case 83: case 115: return (idx_t)27;
        /* Group 16 → column 28 */
        case 8: case 16: case 34: case 52: case 84: case 116: return (idx_t)28;
        /* Group 17 → column 29 */
        case 9: case 17: case 35: case 53: case 85: case 117: return (idx_t)29;
        /* Group 18 → column 30 (excluding He which was handled above) */
        case 10: case 18: case 36: case 54: case 86: case 118: return (idx_t)30;
        /* Group 1 → column 31 */
        case 1: case 3: case 11: case 19: case 37: case 55: case 87: case 119:
            return (idx_t)31;
        /* Group 2 → column 32 */
        case 4: case 12: case 20: case 38: case 56: case 88: case 120:
            return (idx_t)32;
        default:
            return (idx_t)0; /* Should not happen for 1…120 */
    }
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t sums[33] = {0};

    if (!init) {
        for (idx_t i = 1; i <= 120; ++i) {
            idx_t col = get_col(i);
            if (col >= 1 && col <= 32) {
                sums[(size_t)col] += (seq_t)i;
            }
        }
        init = true;
    }
    if (n >= 1 && n <= 32) {
        return sums[(size_t)n];
    }
    return (seq_t)0;
}
